// Fill out your copyright notice in the Description page of Project Settings.
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMTile.h"
#include "HMGrid.h"
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
	Tiles.RemoveAll([](AHMTile* Tile_) {
		return Tile_ == nullptr;
	});
	auto TileIt = TilesToLocationsLinkages.Find(Tile->UUID.HexCoord.ToVec());
	if (!TileIt)
	{
		if (!Tile->UUID.IsUndefined())
		{
			TilesToLocationsLinkages.Add(Tile->UUID.HexCoord.ToVec(), Tile);
		}
	}
	if (Tiles.Find(Tile) == INDEX_NONE)
	{
		Tiles.Add(Tile);
	}
	UpdateTiles();
}

void AHMGrid::OnTileRemoved(class AHMTile* Tile)
{
	if (!Tile->UUID.IsUndefined())
	{
		TilesToLocationsLinkages.Remove(Tile->UUID.HexCoord.ToVec());
	}
	Tiles.Remove(Tile);
	UpdateTiles();
}

void AHMGrid::OnTileLocationUpdated(class AHMTile* Tile, const FHMTileUUID& OldUUID, bool bError)
{
	TilesToLocationsLinkages.Remove(OldUUID.HexCoord.ToVec());
	if (!bError)
	{
		if (!Tile->UUID.IsUndefined())
		{
			TilesToLocationsLinkages.Add(Tile->UUID.HexCoord.ToVec(), Tile);
		}
	}
	UpdateTiles();
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
					UHMMeshComponent::AddHexTileGeometry(Location2D, VisualTileSize, Triangles);
					TrianglesKeys.Add(NeighbourTileHexCoord.ToVec());
				}
			}
			if (!TrianglesKeys.Contains(HexCoord.ToVec()))
			{
				FVector2D Location2D = FHMCoord::ToLocation(Layout, HexCoord);
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

