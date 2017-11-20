// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMUtilities.h"
#include "HMTile.generated.h"

UCLASS()
class HEXMAP_API AHMTile : public AActor
{
	GENERATED_BODY()
	
public:	

	AHMTile();

private:

	friend class AHMGrid;

	bool bConstructed = false;
	bool bShouldBeAttached = false;
	bool bEdit = false;

	TMap<FString, UMaterialInterface*> AttachedMaterials;

	void Mark();

protected:

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

#endif

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;
	virtual void BeginPlay() override;

	virtual void SnapToGrid();
	virtual void OnSnapError(bool bAttached_);
	virtual bool CanBeSnapped();

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UHMTilePropertiesComponent* PropertiesComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	struct FHMTileUUID UUID = FHMTileUUID::Undefined();

	UPROPERTY(EditAnywhere)
	bool bAttached = true;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* DetachedMaterial;

	UPROPERTY(EditAnywhere)
	TArray<AHMTile*> Neighbours;

	UPROPERTY(EditAnywhere)
	TSet<FString> Groups;

	bool IsInGroup(const FString& GroupName) const;

#if WITH_EDITOR

	void OnEditorMousePressed();
	void OnEditorMouseReleased();
	void OnEditorTick(float DeltaTime);

#endif
};
