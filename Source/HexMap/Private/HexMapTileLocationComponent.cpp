// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileLocationComponent.h"

UHexMapTileLocationComponent::UHexMapTileLocationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UHexMapTileLocationComponent::~UHexMapTileLocationComponent()
{

}

void UHexMapTileLocationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHexMapTileLocationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

