// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HMActorNavigationComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHMActorNavigationComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UHMActorNavigationComponent();

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* DebugNavigationMaterial;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* DebugNavigationMesh;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	float SplineInterval = 50.f;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	float SplineZOffset = 50.f;

	TArray<FVector> Solution;
	FVector LastGoalLocation;
	bool bIsHasSolution = false;
	AController* PawnController = nullptr;
	FVector LookAtLocation;

	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual void UpdateSpline(bool bVisible);

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	float AcceptableRadiusBetweenNavigationPoints = 10.f;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	float AcceptableRadiusToLastNavigationPoint = 50.f;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	float AcceptableRadius = 50.f;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	int32 SpreadingRadius = 2;

	UPROPERTY()
	TArray<class AHMTile*> CapturedTiles;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	bool bDebug = false;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	FLinearColor DebugColor = FLinearColor::Green;

	UPROPERTY(Category = "Hex Map", EditAnywhere, BlueprintReadWrite)
	class UHMGridNavigationComponent* GridNavigationComponent = nullptr;

	UFUNCTION(Category = "Hex Map", BlueprintCallable)
	bool GetPath(const FVector& GoalLocation);

	UFUNCTION(Category = "Hex Map", BlueprintCallable)
	void Interrupt();

	UFUNCTION(Category = "Hex Map", BlueprintCallable)
	FVector GetNextNavigationPoint();

	UFUNCTION(Category = "Hex Map", BlueprintCallable)
	bool IsGoalLocationReached(const FVector& GoalLocation, float AcceptableRadius_ = -1.f) const;

	UFUNCTION(Category = "Hex Map", BlueprintCallable)
	bool IsGoalActorReached(AActor* Actor, float AcceptableRadius_ = -1.f) const;

	UFUNCTION(Category = "Hex Map", BlueprintCallable)
	bool MoveToLocation(AController* Controller, const FVector& GoalLocation);

	UFUNCTION(Category = "Hex Map", BlueprintCallable)
	bool MoveToActor(AController* Controller, AActor* Actor);
};
