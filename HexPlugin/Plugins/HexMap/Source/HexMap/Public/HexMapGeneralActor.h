// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexMapGeneralActor.generated.h"

UCLASS()
class HEXMAP_API AHexMapGeneralActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AHexMapGeneralActor();

	static const int k_InitialTileWidth = 100;
	static const int k_InitialTileHeight = 100;

protected:

	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;

	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	virtual void EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

	void OnHexMapTileSizeChanged();

public:	

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	TArray<class AHexMapChunkActor*> HexMapChunkActors;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> HexMapChunkTilePositions;

	UPROPERTY(VisibleAnywhere)
	TMap<FVector, AHexMapChunkActor*> HexMapChunkActorToPositionLinkage;

	UPROPERTY(EditAnywhere)
	int TileWidth = k_InitialTileWidth;

	UPROPERTY(EditAnywhere)
	int TileHeight = k_InitialTileHeight;
	
	void OnHexMapChunkActorChangedLocation();
	void OnHexMapChunkAttachesChanged();
};
