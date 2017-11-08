// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMTileRandomizer.h"

AHMTileRandomizer::AHMTileRandomizer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHMTileRandomizer::BeginPlay()
{
	Super::BeginPlay();
}

void AHMTileRandomizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHMTileRandomizer::Randomize(class AHMTile* Tile)
{

}
