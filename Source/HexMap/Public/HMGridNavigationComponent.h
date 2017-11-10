// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HMGridNavigationComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHMGridNavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHMGridNavigationComponent();

protected:

	TSharedPtr<class FHMNavigation> Navigation;

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	float MaxPassableDifferential = 55.f;

	virtual void SetupNavigation();
	bool GetPath(const FVector& StartLocation, const FVector& GoalLocation, TArray<FVector>& OutSolution, int32 SpreadingRadius = 0);
};
