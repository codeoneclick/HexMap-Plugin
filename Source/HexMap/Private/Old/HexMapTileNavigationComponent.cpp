#include "../HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileNavigationComponent.h"

UHexMapTileNavigationComponent::UHexMapTileNavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHexMapTileNavigationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHexMapTileNavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

