
#include "HMGrid.h"
#include "HMTile.h"
#include "HMMeshComponent.h"

AHMGrid::AHMGrid()
{
	PrimaryActorTick.bCanEverTick = true;
	Layout = FHMLayout::Init(FHMLayout::Flat, FVector2D(TileSize, TileSize), FVector2D(TileSize * .5f, TileSize * .5f));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HMGridComponent"));;

	GeometryComponent = CreateDefaultSubobject<UHMMeshComponent>(TEXT("HMGridMeshComponent"));
	GeometryComponent->SetupAttachment(GetRootComponent());
	GeometryComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHMGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AHMGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHMGrid::OnTileAdded(class AHMTile* Tile)
{
	FHMTileOperation TileOperation;
	TileOperation.Operation = EHMTileOperation::OP_ADD;
	TileOperation.Tile = Tile;
	OperationQueue.Enqueue(TileOperation);
}

void AHMGrid::OnTileRemoved(class AHMTile* Tile)
{
	FHMTileOperation TileOperation;
	TileOperation.Operation = EHMTileOperation::OP_REMOVE;
	TileOperation.Tile = Tile;
	OperationQueue.Enqueue(TileOperation);
}

void AHMGrid::OnTileLocationUpdated(class AHMTile* Tile, const FHMTileUUID& OldUUID, bool bError)
{
	FHMTileOperation TileOperation;
	TileOperation.Operation = EHMTileOperation::OP_UPDATE;
	TileOperation.Tile = Tile;
	TileOperation.OldUUID = OldUUID;
	TileOperation.bError = bError;
	OperationQueue.Enqueue(TileOperation);
}

void AHMGrid::UpdateTiles()
{
	UpdateTilesLogic();
	UpdateTilesVisual();
}

void AHMGrid::UpdateTilesVisual()
{
	float VisualTileSize = TileSize * .9f;
	TArray<FHMMeshTriangle> Triangles;
	TSet<FIntVector> TrianglesKeys;
	for (AHMTile* Tile : Tiles)
	{
		if (Tile->bAttached)
		{
			FHMCoord HexCoord = Tile->UUID.HexCoord;
			for (FHMCoord Direction : FHMCoord::HexDirections)
			{
				FHMCoord NeighbourTileHexCoord = HexCoord;
				NeighbourTileHexCoord.Q += Direction.Q;
				NeighbourTileHexCoord.R += Direction.R;
				NeighbourTileHexCoord.S += Direction.S;

				if (!TrianglesKeys.Contains(NeighbourTileHexCoord.ToVec()))
				{
					FVector2D Location2D = FHMCoord::ToLocation(Layout, NeighbourTileHexCoord);
					Location2D.X -= GetActorLocation().X;
					Location2D.Y -= GetActorLocation().Y;
					UHMMeshComponent::AddHexTileGeometry(Location2D, VisualTileSize, Triangles);
					TrianglesKeys.Add(NeighbourTileHexCoord.ToVec());
				}
			}
			if (!TrianglesKeys.Contains(HexCoord.ToVec()))
			{
				FVector2D Location2D = FHMCoord::ToLocation(Layout, HexCoord);
				Location2D.X -= GetActorLocation().X;
				Location2D.Y -= GetActorLocation().Y;
				UHMMeshComponent::AddHexTileGeometry(Location2D, VisualTileSize, Triangles);
				TrianglesKeys.Add(HexCoord.ToVec());
			}
		}
	}
	GeometryComponent->SetTriangles(Triangles);
}

void AHMGrid::UpdateTilesLogic()
{

}

void AHMGrid::OnTileSizeChanged(float TileSize_)
{
	TileSize = TileSize_;
	Layout = FHMLayout::Init(FHMLayout::Flat, FVector2D(TileSize, TileSize), FVector2D(TileSize * .5f, TileSize * .5f));

	for (auto It : TilesToLocationsLinkages)
	{
		FVector GoalLocation = FHMUtilities::ToSnapLocation(GetWorld(), FHMCoord::Init(It.Key.X, It.Key.Y, It.Key.Z));
		GoalLocation.Z = It.Value->GetActorLocation().Z;
		It.Value->SetActorLocation(GoalLocation);
	}
	UpdateTiles();
}

void AHMGrid::Validate()
{
	TArray<FIntVector> InvalidTiles;
	for (auto TileIt : TilesToLocationsLinkages)
	{
		if (!TileIt.Value || (TileIt.Value && TileIt.Value->UUID.IsUndefined()))
		{
			InvalidTiles.Add(TileIt.Key);
		}
	}
	TArray<AHMTile*> UniqueValidatedTiles;
	TilesToLocationsLinkages.GenerateValueArray(UniqueValidatedTiles);
	UniqueValidatedTiles.RemoveAll([](AHMTile* Tile) {
		return Tile == nullptr;
	});
	UniqueValidatedTiles.Sort([](const AHMTile& LTile, const AHMTile& RTile) {
		return LTile.GetUniqueID() < RTile.GetUniqueID();
	});
	AHMTile* PreviousTile = nullptr;
	FHMTileUUID PreviousUUID = FHMTileUUID::Undefined();
	for (AHMTile* Tile : UniqueValidatedTiles)
	{
		if (Tile == PreviousTile)
		{
			InvalidTiles.Add(PreviousUUID.HexCoord.ToVec());
			InvalidTiles.Add(Tile->UUID.HexCoord.ToVec());
		}
		PreviousTile = Tile;
		PreviousUUID = Tile->UUID;
	}

	for (const FIntVector& Key : InvalidTiles)
	{
		auto TileIt = TilesToLocationsLinkages.Find(Key);
		if (TileIt && (*TileIt))
		{
			(*TileIt)->bAttached = false;
			(*TileIt)->Mark();
		}
		TilesToLocationsLinkages.Remove(Key);
		UE_LOG(LogTemp, Warning, TEXT("Deattached invalid Tile!"));
	}
}

#if WITH_EDITOR

void AHMGrid::OnEditorTick(float DeltaTime)
{
	if (!OperationQueue.IsEmpty())
	{
		FHMTileOperation TileOperation;
		while (!OperationQueue.IsEmpty())
		{
			OperationQueue.Peek(TileOperation);
			ensure(TileOperation.Tile != nullptr);
			switch (TileOperation.Operation)
			{
				case EHMTileOperation::OP_ADD:
				{
					auto TileIt = TilesToLocationsLinkages.Find(TileOperation.Tile->UUID.HexCoord.ToVec());
					if (!TileIt)
					{
						if (!TileOperation.Tile->UUID.IsUndefined())
						{
							TilesToLocationsLinkages.Add(TileOperation.Tile->UUID.HexCoord.ToVec(), TileOperation.Tile);
						}
						else
						{
							ensure(false);
						}
					}
				}
				break;
				case EHMTileOperation::OP_UPDATE:
				{
					TilesToLocationsLinkages.Remove(TileOperation.OldUUID.HexCoord.ToVec());
					if (!TileOperation.bError)
					{
						if (!TileOperation.Tile->UUID.IsUndefined())
						{
							TilesToLocationsLinkages.Add(TileOperation.Tile->UUID.HexCoord.ToVec(), TileOperation.Tile);
						}
					}
				}
				break;
				case EHMTileOperation::OP_REMOVE:
				{
					if (!TileOperation.Tile->UUID.IsUndefined())
					{
						TilesToLocationsLinkages.Remove(TileOperation.Tile->UUID.HexCoord.ToVec());
					}
				}
				break;
				default:
				{
					ensure(false);
				}
				break;
			}
			OperationQueue.Pop();
		}
		Validate();
		TilesToLocationsLinkages.GenerateValueArray(Tiles);
		UpdateTiles();
	}
}

#endif

