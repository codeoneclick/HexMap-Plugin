
#include "HexMapPrivatePCH.h"
#include "HMActorNavigationComponent.h"

UHMActorNavigationComponent::UHMActorNavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHMActorNavigationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHMActorNavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

