// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HMTileNavigationComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHMTileNavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHMTileNavigationComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	bool bIsPassable = true;

	virtual bool IsPassable(class UHMActorNavigationComponent* ActorNavigationComponent) const;
};
