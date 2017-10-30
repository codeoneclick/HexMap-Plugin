// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapNavigationComponent.h"
#include "HexCoord.h"
#include "HexUtils.h"
#include "HexMapGrid.h"
#include "HexMapChunk.h"
#include "HexMapTile.h"
#include "HexMapTileLocationComponent.h"
#include "HexNavigation.h"

UHexMapNavigationComponent::UHexMapNavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHexMapNavigationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHexMapNavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHexMapNavigationComponent::SetupNavigation()
{
	Navigation = MakeShareable(new FHexNavigation());
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	Navigation->SetTileHeight(Grid->TileHeight);
	Navigation->SetMaxPassableHeight(MaxPassableHeight);
	TArray<AHexMapChunk*> Chunks = Grid->Chunks;
	for (AHexMapChunk* Chunk : Chunks)
	{
		TArray<USceneComponent*> ComponentsInRoot;
		Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
		for (USceneComponent* ComponentInRoot : ComponentsInRoot)
		{
			if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
			{
				Navigation->RegisterHexTileLocationComponent(Cast<UHexMapTileLocationComponent>(ComponentInRoot));
			}
		}
	}
	Navigation->UpdateNavigationNodes(true);
}

bool UHexMapNavigationComponent::GetPath(const FVector& StartLocation, const FVector& GoalLocation, TArray<FVector>& OutSolution)
{
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	bool bResult = false;
	FVector StartCenterLocation = FHex::GetTileLocation(Grid->TilesLocations, StartLocation);
	FVector GoalCenterLocation = FHex::GetTileLocation(Grid->TilesLocations, GoalLocation);
	FVector2D StartCenterLocation2D = FVector2D(StartCenterLocation.X, StartCenterLocation.Y);
	FVector2D GoalCenterLocation2D = FVector2D(GoalCenterLocation.X, GoalCenterLocation.Y);

	auto StartChunkIt = Grid->ChunksToLocationsLinkages.Find(StartCenterLocation2D);
	auto GoalChunkIt = Grid->ChunksToLocationsLinkages.Find(GoalCenterLocation2D);
	if (StartChunkIt && GoalChunkIt)
	{
		AHexMapChunk* StartChunk = *StartChunkIt;
		AHexMapChunk* GoalChunk = *GoalChunkIt;

		auto StartTileLocationComponentIt = StartChunk->TileLocationComponentsToLocationsLinkages.Find(StartCenterLocation2D);
		auto GoalTileLocationComponentIt = GoalChunk->TileLocationComponentsToLocationsLinkages.Find(GoalCenterLocation2D);
		if (StartTileLocationComponentIt && GoalTileLocationComponentIt)
		{
			UHexMapTileLocationComponent* StartTileLocationComponent = *StartTileLocationComponentIt;
			UHexMapTileLocationComponent* GoalTileLocationComponent = *GoalTileLocationComponentIt;
			bResult = Navigation->GetPath(StartTileLocationComponent, GoalTileLocationComponent, OutSolution);
		}
	}
	return bResult;
}

