// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileMeshComponent.h"

static unsigned int HexMapTileMeshComponentInstancesCount = 0;

UHexMapTileMeshComponent::UHexMapTileMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	HexMapTileMeshComponentInstancesCount++;
	UE_LOG(LogTemp, Warning, TEXT("UHexMapTileMeshComponent created! Count: %d"), HexMapTileMeshComponentInstancesCount);
}

UHexMapTileMeshComponent::~UHexMapTileMeshComponent()
{
	HexMapTileMeshComponentInstancesCount--;
	UE_LOG(LogTemp, Warning, TEXT("UHexMapTileMeshComponent destroyed! Count: %d"), HexMapTileMeshComponentInstancesCount);
}

void UHexMapTileMeshComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UHexMapTileMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




