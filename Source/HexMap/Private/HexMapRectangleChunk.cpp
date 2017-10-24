// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapRectangleChunk.h"
#include "HexCoord.h"
#include "HexMapGrid.h"
#include "FreeformMeshComponent.h"
#include "HexMapTileLocationComponent.h"

#if WITH_EDITOR

void AHexMapRectangleChunk::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);
	FName PropertyName = (Event.Property != NULL) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapRectangleChunk, SizeX) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapRectangleChunk, SizeY))
	{
		OnSizeChanged();
	}
}

#endif

void AHexMapRectangleChunk::GenerateTilesMetadata(bool bIsReconstruct)
{
	Super::GenerateTilesMetadata(bIsReconstruct);

	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());

	if (bIsReconstruct)
	{
		static FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
		for (int32 i = 0; i < SizeX; ++i)
		{
			for (int32 j = 0; j < SizeY; ++j)
			{
				FHexCoord HexLocation = FHexCoord::QOffsetToCube(FHexCoord::EHexDirection::ODD, FVector2D(i, j));
				FVector2D Location2D = FHexCoord::ToLocation(Grid->Layout, HexLocation);
				CreateTileLocationComponent(HexLocation, Location2D);
			}
		}
	}
	else
	{
		for (int32 i = 0; i < SizeX; ++i)
		{
			for (int32 j = 0; j < SizeY; ++j)
			{
				FHexCoord HexLocation = FHexCoord::QOffsetToCube(FHexCoord::EHexDirection::ODD, FVector2D(i, j));
				FVector2D Location2D = FHexCoord::ToLocation(Grid->Layout, HexLocation);
				UpdateTileLocationComponent(HexLocation, Location2D);
			}
		}
	}
}

void AHexMapRectangleChunk::GenerateTilesGeometry(bool bIsReconstruct)
{
	Super::GenerateTilesGeometry(bIsReconstruct);

	TArray<FFreeformMeshTriangle> Triangles;

	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());

	float TileSize = Grid->TileSize;
	float FreeformGeometryTileSize = TileSize * .9f;

	for (int32 i = 0; i < SizeX; ++i)
	{
		for (int32 j = 0; j < SizeY; ++j)
		{
			FHexCoord HexLocation = FHexCoord::QOffsetToCube(FHexCoord::EHexDirection::ODD, FVector2D(i, j));
			FVector2D Location2D = FHexCoord::ToLocation(Grid->Layout, HexLocation);
			AddHexTileGeometry(Location2D, FreeformGeometryTileSize, Triangles);
		}
	}
	GeometryComponent->SetTriangles(Triangles);
}

#if WITH_EDITOR

void AHexMapRectangleChunk::SetSizeX(int32 aSizeX)
{
	SizeX = aSizeX;
	OnSizeChanged();
}

void AHexMapRectangleChunk::SetSizeY(int32 aSizeY)
{
	SizeY = aSizeY;
	OnSizeChanged();
}

void AHexMapRectangleChunk::SetSize(int32 aSizeX, int32 aSizeY)
{
	SizeX = aSizeX;
	SizeY = aSizeY;
	OnSizeChanged();
}

#endif

