// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HMCoord.h"
#include "HMUtilities.h"
#include "HMActorPlacementComponent.generated.h"

UENUM(BlueprintType)
enum class EPlacementModeEnum : uint8
{
	PM_CIRCLE UMETA(DisplayName = "Circle"),
	PM_RECTANGLE UMETA(DisplayName = "Rectangle"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHMActorPlacementComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UHMActorPlacementComponent();
	bool bConstructed = false;

protected:

	TArray<UHMActorPlacementComponent *> AttachedPlacements;

	virtual void BeginPlay() override;
	virtual void OnChildAttached(USceneComponent* ChildComponent) override;
	virtual void OnChildDetached(USceneComponent* ChildComponent) override;

	void CalculatePlacementsLocations();

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere)
	FHMLayout Layout;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float TileSize = 200.f;

	UPROPERTY(EditAnywhere)
	int32 Radius = 1;

	UPROPERTY(EditAnywhere)
	int32 SizeX = 2;

	UPROPERTY(EditAnywhere)
	int32 SizeY = 2;

	UPROPERTY(EditAnywhere)
	EPlacementModeEnum PlacementMode = EPlacementModeEnum::PM_CIRCLE;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* RootComponent = nullptr;

	UPROPERTY(EditAnywhere)
	class UMaterial* Material = nullptr;
	
	UPROPERTY(EditAnywhere)
	class UStaticMesh* Mesh = nullptr;
};
