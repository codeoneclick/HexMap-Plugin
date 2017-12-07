// Copyright serhii serhiiv 2017. All rights reserved.

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

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	class UHMTilePropertiesComponent* PropertiesComponent = nullptr;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	struct FHMTileUUID UUID = FHMTileUUID::Undefined();

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	bool bAttached = true;

	UPROPERTY()
	UMaterialInterface* DetachedMaterial;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	UMaterialInterface* GlobalMaterial;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	TArray<AHMTile*> Neighbours;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	TSet<FString> Groups;

	bool IsInGroup(const FString& GroupName) const;

#if WITH_EDITOR

	void OnEditorMousePressed();
	void OnEditorMouseReleased();
	void OnEditorTick(float DeltaTime);

#endif
};
