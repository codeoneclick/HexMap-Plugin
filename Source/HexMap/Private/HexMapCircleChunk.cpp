// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexCoord.h"
#include "HexMapGrid.h"
#include "HexMapTile.h"
#include "HexMapCircleChunk.h"
#include "HexMapTileLocationComponent.h"
#include "FreeformMeshComponent.h"

#if WITH_EDITOR

void AHexMapCircleChunk::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);
	FName PropertyName = (Event.Property != NULL) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapCircleChunk, Radius))
	{
		OnSizeChanged();
	}
}

void AHexMapCircleChunk::SetRadius(int32 aRadius)
{
	Radius = aRadius;
	OnSizeChanged();
}

#endif

void AHexMapCircleChunk::GenerateTilesMetadata(bool bIsReconstruct)
{
	Super::GenerateTilesMetadata(bIsReconstruct);

	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());

	if (bIsReconstruct)
	{
		static FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
		for (int32 Q = -Radius; Q <= Radius; ++Q)
		{
			int32 R1 = std::max(-Radius, -Q - Radius);
			int32 R2 = std::min(Radius, -Q + Radius);

			for (int32 R = R1; R <= R2; R++)
			{
				FHexCoord HexLocation = FHexCoord::Init(Q, R, -Q - R);
				FVector2D Location2D = FHexCoord::ToLocation(Grid->Layout, HexLocation);
				CreateTileLocationComponent(HexLocation, Location2D);
			}
		}
	}
	else
	{
		for (int32 Q = -Radius; Q <= Radius; ++Q)
		{
			int32 R1 = std::max(-Radius, -Q - Radius);
			int32 R2 = std::min(Radius, -Q + Radius);

			for (int32 R = R1; R <= R2; R++)
			{
				FHexCoord HexLocation = FHexCoord::Init(Q, R, -Q - R);
				FVector2D Location2D = FHexCoord::ToLocation(Grid->Layout, HexLocation);
				UpdateTileLocationComponent(HexLocation, Location2D);
			}
		}
	}
}

void AHexMapCircleChunk::GenerateTilesGeometry(bool bIsReconstruct)
{
	Super::GenerateTilesGeometry(bIsReconstruct);

	TArray<FFreeformMeshTriangle> Triangles;

	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());

	float TileSize = Grid->TileSize;
	float FreeformGeometryTileSize = TileSize * .9f;

	for (int Q = -Radius; Q <= Radius; ++Q)
	{
		int R1 = std::max(-Radius, -Q - Radius);
		int R2 = std::min(Radius, -Q + Radius);

		for (int R = R1; R <= R2; R++)
		{
			FHexCoord HexLocation = FHexCoord::Init(Q, R, -Q - R);
			FVector2D Location2D = FHexCoord::ToLocation(Grid->Layout, HexLocation);
			AddHexTileGeometry(Location2D, FreeformGeometryTileSize, Triangles);
		}
	}
	GeometryComponent->SetTriangles(Triangles);
}


