// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexMapTileBatchApplier.generated.h"

UCLASS()
class HEXMAP_API AHexMapTileBatchApplier : public AActor
{
	GENERATED_BODY()
	
public:	

	AHexMapTileBatchApplier();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void Apply(class AHexMapTile* Tile);
};
