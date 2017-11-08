// Fill out your copyright notice in the Description page of Project Settings.

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
