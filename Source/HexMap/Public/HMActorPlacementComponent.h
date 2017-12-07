// Copyright serhii serhiiv 2017. All rights reserved.

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

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	FHMLayout Layout;

	UPROPERTY(Category = "Hex Map", EditAnywhere, meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float TileSize = 200.f;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	int32 Radius = 1;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	int32 SizeX = 2;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	int32 SizeY = 2;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	EPlacementModeEnum PlacementMode = EPlacementModeEnum::PM_CIRCLE;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	class UStaticMeshComponent* RootComponent = nullptr;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	class UMaterial* Material = nullptr;
	
	UPROPERTY(Category = "Hex Map", EditAnywhere)
	class UStaticMesh* Mesh = nullptr;

	UPROPERTY()
	TArray<FVector> PlacementsLocations;
};
