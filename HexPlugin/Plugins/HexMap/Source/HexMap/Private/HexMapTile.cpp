// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMapTile.h"

//static int HexMapTilesAllocatedCount = 0;

// Sets default values
AHexMapTile::AHexMapTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//HexMapTilesAllocatedCount++;
	
	//UE_LOG(LogTemp, Warning, TEXT("HexMapTile created! Name: %s, %d"), *GetName(), HexMapTilesAllocatedCount);
}

AHexMapTile::~AHexMapTile()
{
	//HexMapTilesAllocatedCount--;
	//UE_LOG(LogTemp, Warning, TEXT("HexMapTile destroyed!Name: %s, %d"), *GetName(), HexMapTilesAllocatedCount)
}

// Called when the game starts or when spawned
void AHexMapTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHexMapTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

