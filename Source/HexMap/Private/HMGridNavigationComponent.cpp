
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMGrid.h"
#include "HMTile.h"
#include "HMUtilities.h"
#include "HMNavigation.h"
#include "HMTileNavigationComponent.h"
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

bool UHMGridNavigationComponent::GetPath(const FVector& StartLocation, const FVector& GoalLocation, TArray<FVector>& OutSolution, int32 SpreadingRadius)
{
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	bool bResult = false;
	FHMCoord StartHexCoord = FHMUtilities::ToNearestHex(GetWorld(), StartLocation);
	FHMCoord GoalHexCoord = FHMUtilities::ToNearestHex(GetWorld(), GoalLocation);

	AHMTile** StartTileIt = Grid->TilesToLocationsLinkages.Find(StartHexCoord.ToVec());
	AHMTile** GoalTileIt = Grid->TilesToLocationsLinkages.Find(GoalHexCoord.ToVec());
	if (StartTileIt && GoalTileIt)
	{
		AHMTile* StartTile = (*StartTileIt);
		AHMTile* GoalTile = (*GoalTileIt);
		UHMTileNavigationComponent* TileNavigationComponent = GoalTile->FindComponentByClass<UHMTileNavigationComponent>();
		if (TileNavigationComponent && !TileNavigationComponent->bIsPassable && SpreadingRadius > 0)
		{
			TArray<AHMTile*> SpreadingTiles;
			for (int32 Q = -SpreadingRadius; Q <= SpreadingRadius; ++Q)
			{
				int32 R1 = std::max(-SpreadingRadius, -Q - SpreadingRadius);
				int32 R2 = std::min(SpreadingRadius, -Q + SpreadingRadius);

				for (int32 R = R1; R <= R2; R++)
				{
					FHMCoord HexCoord = FHMCoord::Init(Q, R, -Q - R);
					FVector SnapLocation = FHMUtilities::ToSnapLocation(GetWorld(), HexCoord) + GoalLocation;
					FHMCoord SnapHexCoord = FHMUtilities::ToNearestHex(GetWorld(), SnapLocation);
					AHMTile** SpreadingTile = Grid->TilesToLocationsLinkages.Find(SnapHexCoord.ToVec());
					if (SpreadingTile)
					{
						UHMTileNavigationComponent* SpreadingTileNavigationComponent = (*SpreadingTile)->FindComponentByClass<UHMTileNavigationComponent>();
						if (SpreadingTileNavigationComponent && SpreadingTileNavigationComponent->bIsPassable)
						{
							SpreadingTiles.Add((*SpreadingTile));
						}
					}
				}
			}
			if (SpreadingTiles.Num() > 0)
			{
				SpreadingTiles.Sort([GoalTile](const AHMTile& LTile, const AHMTile& RTile) {
					FVector GoalTileLocation = GoalTile->GetActorLocation();
					GoalTileLocation.Z = 0.f;
					FVector LTileLocation = LTile.GetActorLocation();
					LTileLocation.Z = 0.f;
					FVector RTileLocation = RTile.GetActorLocation();
					RTileLocation.Z = 0.f;

					float DistanceToL = FVector::Distance(GoalTileLocation, LTileLocation);
					float DistanceToR = FVector::Distance(GoalTileLocation, RTileLocation);

					return DistanceToL < DistanceToR;
				});

				FVector GoalTileLocation = GoalTile->GetActorLocation();
				GoalTileLocation.Z = 0.f;
				FVector SpreadingTileLocation = SpreadingTiles[0]->GetActorLocation();
				SpreadingTileLocation.Z = 0.f;
				
				GoalTile = SpreadingTiles[0];
				
				float MinDistanceToGoal = FVector::Distance(GoalTileLocation, SpreadingTileLocation);
				float MinDistanceToStart = FVector::Distance(StartLocation, SpreadingTileLocation);
				for (AHMTile* Tile : SpreadingTiles)
				{
					SpreadingTileLocation = Tile->GetActorLocation();
					SpreadingTileLocation.Z = 0.f;
					float CurrentDistanceToGoal = FVector::Distance(GoalTileLocation, SpreadingTileLocation);
					if (FMath::Abs(MinDistanceToGoal - CurrentDistanceToGoal) <= 1.f)
					{
						float CurrentDistanceToStart = FVector::Distance(StartLocation, SpreadingTileLocation);
						if (CurrentDistanceToStart < MinDistanceToStart)
						{
							MinDistanceToStart = CurrentDistanceToStart;
							GoalTile = Tile;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		bResult = Navigation->GetPath(StartTile, GoalTile, OutSolution);
	}
	return bResult;
}

