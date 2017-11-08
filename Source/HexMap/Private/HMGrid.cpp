// Fill out your copyright notice in the Description page of Project Settings.
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMTile.h"
#include "HMGrid.h"
#include "HMMeshComponent.h"

const int32 AHMGrid::NumSubdivisions = 6;

AHMGrid::AHMGrid()
{
	PrimaryActorTick.bCanEverTick = true;
	Layout = FHMLayout::Init(FHMLayout::Flat, FVector2D(TileSize, TileSize), FVector2D(TileSize * .5f, TileSize * .5f));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HMGrid—omponent"));;

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
					AddHexTileGeometry(Location2D, VisualTileSize, Triangles);
					TrianglesKeys.Add(NeighbourTileHexCoord.ToVec());
				}
			}
			if (!TrianglesKeys.Contains(HexCoord.ToVec()))
			{
				FVector2D Location2D = FHMCoord::ToLocation(Layout, HexCoord);
				AddHexTileGeometry(Location2D, VisualTileSize, Triangles);
				TrianglesKeys.Add(HexCoord.ToVec());
			}
		}
	}
	GeometryComponent->SetTriangles(Triangles);
}

void AHMGrid::UpdateTilesLogic()
{

}

void AHMGrid::AddHexTileGeometry(const FVector2D& Location2D, float Size, TArray<struct FHMMeshTriangle>& Triangles)
{
	FVector HexTileCenter = FVector(Location2D.X, Location2D.Y, 0.f);

	TArray<FVector> HexTileVertices;
	TArray<FVector2D> HexTileTexcoords;
	for (float Angle = 0.f; Angle <= FMath::DegreesToRadians(360.f); Angle += (FMath::DegreesToRadians(360.f) / NumSubdivisions))
	{
		HexTileVertices.Add(FVector(Size * cosf(Angle) + Location2D.X, Size * sinf(Angle) + Location2D.Y, 0.f));
		HexTileTexcoords.Add(FVector2D((cosf(Angle) + 1.f) * .5f, (sinf(Angle) + 1.f) * .5f));
	}

	FHMMeshTriangle Triangle;
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

