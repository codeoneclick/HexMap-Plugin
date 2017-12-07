// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMTileRandomizer.generated.h"

UCLASS()
class HEXMAP_API AHMTileRandomizer : public AActor
{
	GENERATED_BODY()
	
public:	

	AHMTileRandomizer();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void Randomize(class AHMTile* Tile);
};
