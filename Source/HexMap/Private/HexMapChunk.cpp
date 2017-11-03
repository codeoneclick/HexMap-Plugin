// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMapChunk.h"
#include "FreeformMeshComponent.h"
#include "HexMapGrid.h"
#include "HexMapTile.h"
#include "HexCoord.h"
#include "HexMapTileLocationComponent.h"
#include "FHexMapEdMode.h"

int32 AHexMapChunk::GUID = 0;
const int32 AHexMapChunk::NumSubdivisions = 6;

AHexMapChunk::AHexMapChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HexMapChunkComponent"));;

	GeometryComponent = CreateDefaultSubobject<UFreeformMeshComponent>(TEXT("HexMapChunkGeometryComponent"));
	GeometryComponent->SetupAttachment(GetRootComponent());
	GeometryComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHexMapChunk::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!bIsConstructed)
	{
		GUID++;
		UUID = GUID;
		bool bError = false;
		AHexMapGrid* Grid = FHex::GetGrid(GetWorld(), bError);
		if (!bError)
		{
			if (!Grid)
			{
				Grid = GetWorld()->SpawnActor<AHexMapGrid>();
			}
			Grid->OnChunkAdded(this);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Can't Attach HexMapChunk to Unknown HexMapGrid!");
		}
		GenerateTiles(true);
		Snap();
		bIsConstructed = true;
	}
}

void AHexMapChunk::Destroyed()
{
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	if (Grid)
	{
		Grid->OnChunkRemoved(this);
	}

	TArray<USceneComponent*> ComponentsInRoot;
	GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
	for (USceneComponent* ComponentInRoot : ComponentsInRoot)
	{
		if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
		{
			UHexMapTileLocationComponent* TileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
			if (TileLocationComponent->LinkedTile)
			{
				TileLocationComponent->LinkedTile->Destroy();
			}
		}
	}
	Super::Destroyed();
}

void AHexMapChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AHexMapChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR

void AHexMapChunk::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	if (GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap))
	{
		Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
		OnLocationChanged();
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "You can move Chunk only in HexMap Mode!");
	}
}

void AHexMapChunk::EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{

}

#endif

void AHexMapChunk::OnLocationChanged()
{
	TileLocationComponentsToLocationsLinkages.Empty();
	TArray<USceneComponent*> ComponentsInRoot;
	GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
	for (USceneComponent* ComponentInRoot : ComponentsInRoot)
	{
		if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
		{
			FVector CurrentLocation = ComponentInRoot->GetComponentLocation();
			TileLocationComponentsToLocationsLinkages.Add(FVector2D(CurrentLocation.X, CurrentLocation.Y), Cast<UHexMapTileLocationComponent>(ComponentInRoot));
		}
	}
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	Grid->OnChunkLocationChanged();
}

void AHexMapChunk::OnSizeChanged()
{
	GenerateTiles(true);
}

void AHexMapChunk::OnTileSizeChanged(bool bIsReconstruct)
{
	GenerateTiles(false);
}

void AHexMapChunk::GenerateTiles(bool bIsReconstruct)
{
	GenerateTilesGeometry(bIsReconstruct);
	GenerateTilesMetadata(bIsReconstruct);
	OnLocationChanged();
}

void AHexMapChunk::GenerateTilesMetadata(bool bIsReconstruct)
{
	if (bIsReconstruct)
	{
		static FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepRelative, false);
		TArray<USceneComponent*> ComponentsInRoot;
		GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
		for (USceneComponent* ComponentInRoot : ComponentsInRoot)
		{
			if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
			{
				ComponentInRoot->DestroyComponent();
				ComponentInRoot->SetActive(false);
			}
		}
		TileLocationComponentsToHexCoordsLinkages.Empty();
	}
}

void AHexMapChunk::GenerateTilesGeometry(bool bIsReconstruct)
{

}

#if WITH_EDITOR

void AHexMapChunk::OnEditorMousePressed()
{

}

void AHexMapChunk::OnEditorMouseReleased()
{
	if (IsSelectedInEditor())
	{
		Snap();
	}
}

#endif

void AHexMapChunk::CreateTileLocationComponent(const FHexCoord& HexLocation, const FVector2D& Location2D)
{
	UHexMapTileLocationComponent* TileLocationComponent = NewObject<UHexMapTileLocationComponent>(this);
	TileLocationComponent->SetupAttachment(GetRootComponent());
	TileLocationComponent->SetRelativeLocation(FVector(Location2D.X, Location2D.Y, 0.f));
	TileLocationComponent->RegisterComponent();

	TileLocationComponent->UUID = FTileUUID::Init(UUID, HexLocation);
	TileLocationComponentsToHexCoordsLinkages.Add(HexLocation.ToVec(), TileLocationComponent);
}

void AHexMapChunk::UpdateTileLocationComponent(const FHexCoord& HexLocation, const FVector2D& Location2D)
{
	auto TileLocationComponentIt = TileLocationComponentsToHexCoordsLinkages.Find(HexLocation.ToVec());
	if (TileLocationComponentIt)
	{
		UHexMapTileLocationComponent* TileLocationComponent = *TileLocationComponentIt;
		TileLocationComponent->SetRelativeLocation(FVector(Location2D.X, Location2D.Y, 0.f));
		if (TileLocationComponent->LinkedTile)
		{
			AHexMapTile* Tile = TileLocationComponent->LinkedTile;
			FVector TileLocation = Tile->GetActorLocation();
			FVector TileLocationComponentntLocation = TileLocationComponent->GetComponentLocation();
			TileLocation.X = TileLocationComponentntLocation.X;
			TileLocation.Y = TileLocationComponentntLocation.Y;
			Tile->SetActorLocation(TileLocation);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find TileLocationComponent"));
	}
}

void AHexMapChunk::AddHexTileGeometry(const FVector2D& Location2D, float Size, TArray<struct FFreeformMeshTriangle>& Triangles)
{
	FVector HexTileCenter = FVector(Location2D.X, Location2D.Y, 0.f);

	TArray<FVector> HexTileVertices;
	TArray<FVector2D> HexTileTexcoords;
	for (float Angle = 0.f; Angle <= 3.14f * 2.f; Angle += ((3.14f * 2) / NumSubdivisions))
	{
		HexTileVertices.Add(FVector(Size * cosf(Angle) + Location2D.X, Size * sinf(Angle) + Location2D.Y, 0.f));
		HexTileTexcoords.Add(FVector2D((cosf(Angle) + 1.f) * .5f, (sinf(Angle) + 1.f) * .5f));
	}

	FFreeformMeshTriangle Triangle;
	Triangle.Vertices.SetNum(3);
	int32 HexTileVerticesIndex = 0;
	for (int32 i = 0; i < NumSubdivisions; ++i)
	{
		Triangle.Vertices[0].Position = HexTileCenter;
		Triangle.Vertices[0].U = .5f;
		Triangle.Vertices[0].V = .5f;
		HexTileVerticesIndex++;
		Triangle.Vertices[1].Position = HexTileVertices[HexTileVerticesIndex];
		Triangle.Vertices[1].U = HexTileTexcoords[HexTileVerticesIndex].X;
		Triangle.Vertices[1].V = HexTileTexcoords[HexTileVerticesIndex].Y;
		if (HexTileVerticesIndex >= HexTileVertices.Num())
		{
			HexTileVerticesIndex = 0;
		}
		HexTileVerticesIndex--;
		Triangle.Vertices[2].Position = HexTileVertices[HexTileVerticesIndex];
		Triangle.Vertices[2].U = HexTileTexcoords[HexTileVerticesIndex].X;
		Triangle.Vertices[2].V = HexTileTexcoords[HexTileVerticesIndex].Y;
		HexTileVerticesIndex++;

		Triangles.Add(Triangle);
	}
}

void AHexMapChunk::Snap()
{
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	FVector CurrentLocation = GetActorLocation();
	FFractionalHexCoord FractionalHexCoord = FHexCoord::ToHex(Grid->Layout, FVector2D(CurrentLocation.X, CurrentLocation.Y));
	FVector2D Location2D = FHexCoord::ToLocation(Grid->Layout, FHexCoord::Round(FractionalHexCoord));
	FVector SnapLocation = FVector(Location2D.X, Location2D.Y, CurrentLocation.Z);
	SetActorLocation(SnapLocation);
	OnLocationChanged();
}

