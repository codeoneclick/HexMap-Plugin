// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HMPawnNavigationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHMPawnNavigationComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UHMPawnNavigationComponent();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* DebugNavigationMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* DebugNavigationMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SplineInterval = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SplineZOffset = 50.f;

	TArray<FVector> Solution;
	FVector LastGoalLocation;
	bool bIsHasSolution = false;
	AController* PawnController = nullptr;

	virtual void BeginPlay() override;

	virtual void UpdateSpline(bool bVisible);

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AcceptableRadiusBetweenNavigationPoints = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AcceptableRadiusToLastNavigationPoint = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AcceptableRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpreadingRadius = 2;

	UPROPERTY(EditAnywhere)
	TArray<class AHMTile*> CapturedTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DebugColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UHMGridNavigationComponent* GridNavigationComponent = nullptr;

	UFUNCTION(BlueprintCallable)
	bool GetPath(const FVector& GoalLocation);

	UFUNCTION(BlueprintCallable)
	void Interrupt();

	UFUNCTION(BlueprintCallable)
	FVector GetNextNavigationPoint();

	UFUNCTION(BlueprintCallable)
	bool IsGoalReached(const FVector& GoalLocation, float AcceptableRadius_ = -1.f) const;

	UFUNCTION(BlueprintCallable)
	bool MoveToLocation(AController* Controller, const FVector& GoalLocation);
};
