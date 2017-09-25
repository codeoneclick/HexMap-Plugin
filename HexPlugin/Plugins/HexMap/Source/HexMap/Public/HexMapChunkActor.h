// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexMapChunkActor.generated.h"

UCLASS()
class HEXMAP_API AHexMapChunkActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AHexMapChunkActor();

protected:

	bool bIsCreated = false;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;

	void CreateTiles();
	void DestroyTiles();

	void OnHexMapTileMeshChanged();
	void OnHexMapTileSizeChanged();

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UHexMapComponent* HexMapComponent;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 8))
	int MapRadius = 2;

	UPROPERTY(EditAnywhere)
	int TileWidth = 32;

	UPROPERTY(EditAnywhere)
	int TileHeight = 32;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* HexMapTileMesh;
};
