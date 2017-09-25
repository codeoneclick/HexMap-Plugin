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

protected:

	virtual void BeginPlay() override;

	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	virtual void EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

public:	

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	TArray<class AHexMapChunkActor*> HexMapChunkActors;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> HexMapChunkTilePositions;
	
	void OnHexMapChunkActorChangedLocation();
};
