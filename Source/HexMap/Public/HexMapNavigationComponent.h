// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexMapNavigationComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHexMapNavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHexMapNavigationComponent();

protected:

	virtual void BeginPlay() override;

	TSharedPtr<class FHexNavigation> Navigation;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupNavigation();
	bool GetPath(const FVector& StartLocation, const FVector& GoalLocation, TArray<FVector>& OutSolution);
};
