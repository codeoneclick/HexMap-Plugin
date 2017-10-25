// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HexMapEdModeProperties.generated.h"

UCLASS()
class HEXMAP_API UHexMapEdModeCreateCircleChunkProperties : public UObject
{
	GENERATED_BODY()
	
public:

	~UHexMapEdModeCreateCircleChunkProperties();

	UPROPERTY(Category = "Tile Blueprint", EditAnywhere, meta = (ShowForTools = "Tool_Tile_BP"), BlueprintReadWrite)
	TSubclassOf<class AHexMapTile> Tile_BP;

	UPROPERTY(Category = "Radius", EditAnywhere, meta = (ShowForTools = "Tool_Radius", ClampMin = 0, ClampMax = 8), BlueprintReadWrite)
	int32 Radius = 2;

	UPROPERTY(Category = "Location", EditAnywhere, meta = (ShowForTools = "Tool_Location"), BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
};

UCLASS()
class HEXMAP_API UHexMapEdModeCreateRectangleChunkProperties : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Tile Blueprint", EditAnywhere, meta = (ShowForTools = "Tool_Tile_BP"), BlueprintReadWrite)
	TSubclassOf<class AHexMapTile> Tile_BP;

	UPROPERTY(Category = "SizeX", EditAnywhere, meta = (ShowForTools = "Tool_Size", ClampMin = 1, ClampMax = 16), BlueprintReadWrite)
	int32 SizeX = 2;

	UPROPERTY(Category = "SizeY", EditAnywhere, meta = (ShowForTools = "Tool_Size", ClampMin = 1, ClampMax = 16), BlueprintReadWrite)
	int32 SizeY = 2;

	UPROPERTY(Category = "Location", EditAnywhere, meta = (ShowForTools = "Tool_Location"), BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
};

UCLASS()
class HEXMAP_API UHexMapEdModeFillSelectedChunksProperties : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Tile Blueprint", EditAnywhere, meta = (ShowForTools = "Tool_Tile_BP"), BlueprintReadWrite)
	TSubclassOf<class AHexMapTile> Tile_BP;
};

UCLASS()
class HEXMAP_API UHexMapEdModeAddTileProperties : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Tile Blueprint", EditAnywhere, meta = (ShowForTools = "Tool_Tile_BP"), BlueprintReadWrite)
	TSubclassOf<class AHexMapTile> Tile_BP;
};

UCLASS()
class HEXMAP_API UHexMapEdModeRandomizeTilesProperties : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Randomizer Blueprint", EditAnywhere, meta = (ShowForTools = "Tool_Randomizer_BP"), BlueprintReadWrite)
	TSubclassOf<class AHexMapTileRandomizer> Randomizer_BP;
};

UCLASS()
class HEXMAP_API UHexMapEdModeSetTileSizeProperties : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Tile Size", EditAnywhere, meta = (ShowForTools = "Tool_Tile_Size", ClampMin = 1.f, ClampMax = 1000.f), BlueprintReadWrite)
	float TileSize = 200.f;
};