// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMCoord.h"
#include "HMUtilities.h"
#include "HMGrid.generated.h"

UCLASS()
class HEXMAP_API AHMGrid : public AActor
{
	GENERATED_BODY()
	
public:	

	AHMGrid();

private:

	friend class AHMTile;

	static const int32 NumSubdivisions;

	void UpdateTiles();

	void UpdateTilesLogic();
	void UpdateTilesVisual();

	void AddHexTileGeometry(const FVector2D& Location2D, float Size, TArray<struct FHMMeshTriangle>& Triangles);

protected:

	virtual void BeginPlay() override;

	void OnTileAdded(class AHMTile* Tile);
	void OnTileRemoved(class AHMTile* Tile);
	void OnTileLocationUpdated(class AHMTile* Tile, const FHMTileUUID& OldUUID, bool bError = false);

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class UHMMeshComponent* GeometryComponent;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float TileSize = 200.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float TileHeight = 400.f;

	UPROPERTY(VisibleAnywhere)
	FHMLayout Layout;

	UPROPERTY(VisibleAnywhere)
	TArray<class AHMTile*> Tiles;

	UPROPERTY(VisibleAnywhere)
	TMap<FIntVector, class AHMTile*> TilesToLocationsLinkages;

	void OnTileSizeChanged(float TileSize_);
};
