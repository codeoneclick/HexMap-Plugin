
#include "../HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileBatchApplier.h"

AHexMapTileBatchApplier::AHexMapTileBatchApplier()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHexMapTileBatchApplier::BeginPlay()
{
	Super::BeginPlay();
}

void AHexMapTileBatchApplier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexMapTileBatchApplier::Apply(class AHexMapTile* Tile)
{

}

