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

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	virtual void EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

#endif

	void CreateTiles();
	void DestroyTiles();

	void OnHexMapChunkActorChangedLocation();

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UHexMapComponent* HexMapComponent;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 8))
	int MapRadius = 2;

	UPROPERTY(VisibleAnywhere)
	int TileWidth;

	UPROPERTY(VisibleAnywhere)
	int TileHeight;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* HexMapTileMesh;

	UPROPERTY(EditAnywhere)
	class UMaterial* HexMapTileMaterial;

	UPROPERTY(VisibleAnywhere, Category = Materials)
	class UFreeformMeshComponent* Geometry;


	void OnHexMapTileMeshChanged();
	void OnHexMapTileMaterialChanged();
	void OnHexMapTileSizeChanged();
};
