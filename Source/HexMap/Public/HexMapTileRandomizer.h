// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexMapTileRandomizer.generated.h"

UCLASS()
class HEXMAP_API AHexMapTileRandomizer : public AActor
{
	GENERATED_BODY()
	
public:	

	AHexMapTileRandomizer();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void Randomize(class AHexMapTile* Tile);
};
