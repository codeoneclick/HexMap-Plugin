
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMGrid.h"
#include "HMTile.h"
#include "HMUtilities.h"
#include "HMNavigation.h"
#include "HMGridNavigationComponent.h"

UHMGridNavigationComponent::UHMGridNavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHMGridNavigationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHMGridNavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHMGridNavigationComponent::SetupNavigation()
{
	Navigation = MakeShareable(new FHMNavigation());
	
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());

	Navigation->SetTileHeight(Grid->TileHeight);
	Navigation->SetMaxPassableDifferential(MaxPassableDifferential);
	TArray<AHMTile*> Tiles = Grid->Tiles;

	for (AHMTile* Tile : Tiles)
	{
		if (Tile->bAttached)
		{
			Navigation->AddTile(Tile);
		}
	}
	Navigation->ConstructNavigation(true);
}

bool UHMGridNavigationComponent::GetPath(const FVector& StartLocation, const FVector& GoalLocation, TArray<FVector>& OutSolution)
{
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	bool bResult = false;
	FHMCoord StartHexCoord = FHMUtilities::ToHex(GetWorld(), StartLocation);
	FHMCoord GoalHexCoord = FHMUtilities::ToHex(GetWorld(), GoalLocation);

	AHMTile** StartTile = Grid->TilesToLocationsLinkages.Find(StartHexCoord.ToVec());
	AHMTile** GoalTile = Grid->TilesToLocationsLinkages.Find(GoalHexCoord.ToVec());
	if (StartTile && GoalTile)
	{
		bResult = Navigation->GetPath((*StartTile), (*GoalTile), OutSolution);
	}
	return bResult;
}

