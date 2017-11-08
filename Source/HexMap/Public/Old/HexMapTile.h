// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexMapTileLocationComponent.h"
#include "HexMapTile.generated.h"

UCLASS()
class HEXMAP_API AHexMapTile : public AActor
{
	GENERATED_BODY()
	
public:	

	AHexMapTile();

private:

	bool bIsConstructed = false;
	FVector PreviousLocation;

protected:

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

#endif

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void OnEditorMousePressed();
	void OnEditorMouseReleased();
	void SnapToGrid(bool bValidationDialog = false);

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* GeometryComponent;

	UPROPERTY(EditAnywhere)
	bool bIsClear = true;

	UPROPERTY(EditAnywhere)
	bool bIsSnapYToGrid = false;

	UPROPERTY(VisibleAnywhere)
	struct FTileUUID UUID;
};
