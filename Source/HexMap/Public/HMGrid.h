// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMCoord.h"
#include "HMUtilities.h"
#include "Containers/Queue.h"
#include "HMGrid.generated.h"

UENUM(BlueprintType)
enum class EHMTileOperation : uint8
{
	OP_UNKNOWN UMETA(DisplayName = "Unknown"),
	OP_ADD UMETA(DisplayName = "Add"),
	OP_UPDATE UMETA(DisplayName = "Update"),
	OP_REMOVE UMETA(DisplayName = "Remove"),
};

USTRUCT(Blueprintable, BlueprintType)
struct HEXMAP_API FHMTileOperation
{
	GENERATED_USTRUCT_BODY()

public:

	class AHMTile* Tile = nullptr;
	EHMTileOperation Operation = EHMTileOperation::OP_UNKNOWN;
	FHMTileUUID CurrentUUID = FHMTileUUID::Undefined();
	FHMTileUUID OldUUID = FHMTileUUID::Undefined();
	bool bError = false;
};

UCLASS()
class HEXMAP_API AHMGrid : public AActor
{
	GENERATED_BODY()
	
public:	

	AHMGrid();

private:

	friend class AHMTile;

	void UpdateTiles();

	void UpdateTilesLogic();
	void UpdateTilesVisual();

	TQueue<FHMTileOperation> OperationQueue;

protected:

	virtual void BeginPlay() override;

	void OnTileAdded(class AHMTile* Tile);
	void OnTileRemoved(class AHMTile* Tile);
	void OnTileLocationUpdated(class AHMTile* Tile, const FHMTileUUID& OldUUID, bool bError = false);

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	FHMLayout Layout;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float TileSize = 200.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float TileHeight = 400.f;

	UPROPERTY()
	class UHMMeshComponent* GeometryComponent;

	UPROPERTY(VisibleAnywhere)
	TArray<class AHMTile*> Tiles;

	UPROPERTY(VisibleAnywhere)
	TMap<FIntVector, class AHMTile*> TilesToLocationsLinkages;

	void OnTileSizeChanged(float TileSize_);

#if WITH_EDITOR

	void OnEditorTick(float DeltaTime);

#endif
};
