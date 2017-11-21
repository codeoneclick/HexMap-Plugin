
#include "HMTileNavigationComponent.h"
#include "Engine.h"

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

bool UHMTileNavigationComponent::IsPassable(class UHMActorNavigationComponent* ActorNavigationComponent) const
{
	return bIsPassable;
}

void UHMTileNavigationComponent::RemoveNavigationReferences()
{

}

