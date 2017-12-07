// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HMGridPropertiesComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHMGridPropertiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHMGridPropertiesComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	TArray<FVector> TilesLocations;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	TArray<FVector> ClearTilesLocations;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float TileSize;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float TileHeight;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	float MinPossibleDifferential = 10.f;

	virtual void SetupBaseProperties();
	virtual void SetupTilesProperties();
	virtual void SetupTilesNeighbours();

	virtual void GetTileSnapLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, FVector& OutLocation, bool& bIsClear);
	virtual void GetTileSnapLocations(const TArray<FVector>& SourceLocations, int32 LocationsNum, const FVector& CurrentLocation, TArray<FVector>& OutClearLocations, TArray<FVector>& OutUnClearLocations, FVector& OutCenterLocation);

	virtual void GetTileSnapClearLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, FVector& OutLocation, float& OutDistance);
	virtual void GetTileSnapClearLocations(const TArray<FVector>& SourceLocations, int32 ReqLocationsNum, bool bIgnoreHeight, const FVector& CurrentLocation, TArray<FVector>& OutLocations, FVector& OutCenterLocation, float& OutDistanceToCenter);

	virtual class AHMTile* GetTileByLocation(const FVector& Location);
	virtual TArray<class AHMTile*> GetTilesByLocationInRange(const FVector& Location, int32 Radius);
};
