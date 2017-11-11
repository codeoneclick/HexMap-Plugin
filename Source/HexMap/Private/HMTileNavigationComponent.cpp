
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMTileNavigationComponent.h"

UHMTileNavigationComponent::UHMTileNavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHMTileNavigationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHMTileNavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UHMTileNavigationComponent::IsPassable(class UHMPawnNavigationComponent* PawnNavigationComponent) const
{
	return bIsPassable;
}

