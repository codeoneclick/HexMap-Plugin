// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexMapGrid.generated.h"

UCLASS()
class HEXMAP_API AHexMapGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexMapGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly)
	class UChildActorComponent* TilesContainer;

	UPROPERTY(VisibleDefaultsOnly)
	class UChildActorComponent* Tile_01;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UChildActorComponent* Tile_02;

	UPROPERTY(EditAnywhere)
	class UHexMapTileMeshesComponent* HexMapTileMeshesComponent;
};
