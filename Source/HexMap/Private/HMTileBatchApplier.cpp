
#include "HexMapPrivatePCH.h"
#include "HMTileBatchApplier.h"

AHMTileBatchApplier::AHMTileBatchApplier()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHMTileBatchApplier::BeginPlay()
{
	Super::BeginPlay();
}

void AHMTileBatchApplier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHMTileBatchApplier::Apply(class AHMTile* Tile)
{

}
