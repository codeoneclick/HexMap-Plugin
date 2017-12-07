// Copyright serhii serhiiv 2017. All rights reserved.

#include "HMTileRandomizer.h"
#include "Engine.h"

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
