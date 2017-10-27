// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexCoord.h"
#include "HexMapGrid.generated.h"

UCLASS()
class HEXMAP_API AHexMapGrid : public AActor
{
	GENERATED_BODY()
	
public:	

	AHexMapGrid();

private:

	void OnTileSizeChanged();

protected:

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	virtual void EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

#endif

	virtual void UpdateTileNeighbours(class UHexMapTileLocationComponent* TileLocationComponent);
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void OnChunkLocationChanged();

	void OnChunkAdded(class AHexMapChunk* Chunk);
	void OnChunkRemoved(class AHexMapChunk* Chunk);

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> TilesLocations;

	UPROPERTY(VisibleAnywhere)
	TArray<class AHexMapChunk*> Chunks;

	UPROPERTY(VisibleAnywhere)
	TMap<FVector2D, class AHexMapChunk*> ChunksToLocationsLinkages;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHexMapTile> Tile_BP;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float TileSize = 200.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float TileHeight = 400.f;

	UPROPERTY(VisibleAnywhere)
	FHexLayout Layout;

	UPROPERTY(EditAnywhere)
	float NeigboursTileDistanceTrashhold = 1.1f;

#if WITH_EDITOR

	void SetTileSize(float TileSize_);

#endif
};
