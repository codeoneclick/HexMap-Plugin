// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMTileBatchApplier.generated.h"

UCLASS()
class HEXMAP_API AHMTileBatchApplier : public AActor
{
	GENERATED_BODY()
	
public:	

	AHMTileBatchApplier();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void Apply(class AHMTile* Tile);
};
