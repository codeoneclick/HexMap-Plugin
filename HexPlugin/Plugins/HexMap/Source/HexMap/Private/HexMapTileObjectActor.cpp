// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileObjectActor.h"
#include "HexMapGeneralActor.h"

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

void AHexMapTileObjectActor::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	for (TActorIterator<AHexMapGeneralActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AHexMapGeneralActor actor found!"));
		AHexMapGeneralActor* HexMapGeneralActor = *ActorItr;
		FVector MoveDirection = DeltaTranslation;
		MoveDirection.Normalize();
		MoveDirection.X *= HexMapGeneralActor->TileWidth * 1.5f;
		MoveDirection.Y *= HexMapGeneralActor->TileHeight * 1.5f;
		FVector CurrentPosition = GetActorLocation();
		FVector PredictedPosition = CurrentPosition + MoveDirection;
		TArray<FVector> HexMapChunkTilePositions = HexMapGeneralActor->HexMapChunkTilePositions;
		FVector NearestHexMapChunkTilePosition = FVector();
		float NearestDistance = 2048.f;
		for (int HexMapChunkTilePositionIndex = 0; HexMapChunkTilePositionIndex < HexMapChunkTilePositions.Num(); ++HexMapChunkTilePositionIndex)
		{
			float CurrentDistance = PredictedPosition.Distance(PredictedPosition, HexMapChunkTilePositions[HexMapChunkTilePositionIndex]);
			if (CurrentDistance < NearestDistance)
			{
				NearestDistance = CurrentDistance;
				NearestHexMapChunkTilePosition = HexMapChunkTilePositions[HexMapChunkTilePositionIndex];
			}
		}
		if (NearestDistance < (HexMapGeneralActor->TileWidth + HexMapGeneralActor->TileHeight) * .5f * 1.5f)
		{
			Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("AHexMapTileObjectActor translated in editor!"));
}

void AHexMapTileObjectActor::OnEditorMousePressed()
{

}

void AHexMapTileObjectActor::OnEditorMouseReleased()
{
	for (TActorIterator<AHexMapGeneralActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AHexMapGeneralActor actor found!"));
		AHexMapGeneralActor* HexMapGeneralActor = *ActorItr;
		FVector CurrentPosition = GetActorLocation();
		TArray<FVector> HexMapChunkTilePositions = HexMapGeneralActor->HexMapChunkTilePositions;
		FVector NearestHexMapChunkTilePosition = FVector();
		float NearestDistance = 2048.f;
		for (int HexMapChunkTilePositionIndex = 0; HexMapChunkTilePositionIndex < HexMapChunkTilePositions.Num(); ++HexMapChunkTilePositionIndex)
		{
			float CurrentDistance = CurrentPosition.Distance(CurrentPosition, HexMapChunkTilePositions[HexMapChunkTilePositionIndex]);
			if (CurrentDistance < NearestDistance)
			{
				NearestDistance = CurrentDistance;
				NearestHexMapChunkTilePosition = HexMapChunkTilePositions[HexMapChunkTilePositionIndex];
			}
		}
		SetActorLocation(NearestHexMapChunkTilePosition);
	}
	UE_LOG(LogTemp, Warning, TEXT("AHexMapTileObjectActor OnEditorMouseReleased!"));
}

