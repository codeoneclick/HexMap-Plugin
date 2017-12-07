// Copyright serhii serhiiv 2017. All rights reserved.

#include "HMTilePropertiesComponent.h"
#include "Engine.h"

UHMTilePropertiesComponent::UHMTilePropertiesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHMTilePropertiesComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHMTilePropertiesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

