
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMTilePropertiesComponent.h"

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

