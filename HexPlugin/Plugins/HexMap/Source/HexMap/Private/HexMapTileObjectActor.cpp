// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileObjectActor.h"
#include "HexMapGrid.h"

// Sets default values
AHexMapTileObjectActor::AHexMapTileObjectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHexMapTileObjectActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHexMapTileObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexMapTileObjectActor::EditorApplyTranslation
(
	const FVector & DeltaTranslation,
	bool bAltDown,
	bool bShiftDown,
	bool bCtrlDown
)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
	for (TActorIterator<AHexMapGrid> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AHexMapGrid actor found!"));
	}
	UE_LOG(LogTemp, Warning, TEXT("AHexMapTileObjectActor translated in editor!"));
}

