// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexMapGridComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHexMapGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHexMapGridComponent();

protected:

	virtual void BeginPlay() override;

	TArray<FVector> ClearTilesLocations;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupBaseProperties();
	virtual void SetupTilesProperties();
	virtual void SetupTilesNeighbours();

	virtual void GetHexGridSnapLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, FVector& OutLocation, bool& bIsClear);
	virtual void GetHexGridSnapLocations(const TArray<FVector>& SourceLocations, int32 LocationsNum, const FVector& CurrentLocation, TArray<FVector>& OutClearLocations, TArray<FVector>& OutUnClearLocations, FVector& OutCenterLocation);
	
	virtual void GetHexGridSnapClearLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, FVector& OutLocation, float& OutDistance);
	virtual void GetHexGridSnapClearLocations(const TArray<FVector>& SourceLocations, int32 ReqLocationsNum, bool bIgnoreHeight, const FVector& CurrentLocation, TArray<FVector>& OutLocations, FVector& OutCenterLocation, float& OutDistanceToCenter);

	virtual const TArray<FVector>& GetClearTilesLocations() const;

	virtual FVector GetTileLocation(const FVector& Location) const;
	virtual class AHexMapTile* GetTileByLocation(const FVector& Location) const;
	virtual TArray<class AHexMapTile*> GetTilesByLocationInRange(const FVector& Location, int Radius);

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> TilesLocations;

	UPROPERTY(VisibleAnywhere)
	float TileSize;

	UPROPERTY(VisibleAnywhere)
	float TileHeight;

	UPROPERTY(VisibleAnywhere)
	float TileHeightThreshold = 10.f;
};
