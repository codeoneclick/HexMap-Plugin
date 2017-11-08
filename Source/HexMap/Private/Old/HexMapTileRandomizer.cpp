// Fill out your copyright notice in the Description page of Project Settings.

#include "../HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileRandomizer.h"

AHexMapTileRandomizer::AHexMapTileRandomizer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHexMapTileRandomizer::BeginPlay()
{
	Super::BeginPlay();
}

void AHexMapTileRandomizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexMapTileRandomizer::Randomize(class AHexMapTile* Tile)
{

}

