// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileComponent.h"
#include "HexMapTileMeshComponent.h"

static unsigned int HexMapTileComponentInstancesCount = 0;

UHexMapTileComponent::UHexMapTileComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	HexMapTileComponentInstancesCount++;
	UE_LOG(LogTemp, Warning, TEXT("UHexMapTileComponent created! Count: %d"), HexMapTileComponentInstancesCount);
}

UHexMapTileComponent::~UHexMapTileComponent()
{
	HexMapTileComponentInstancesCount--;
	UE_LOG(LogTemp, Warning, TEXT("UHexMapTileComponent destroyed! Count: %d"), HexMapTileComponentInstancesCount);
}

void UHexMapTileComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHexMapTileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

