
#include "HMGridPropertiesComponent.h"
#include "Engine.h"
#include "HMUtilities.h"
#include "HMCoord.h"
#include "HMGrid.h"
#include "HMTile.h"
#include "HMTilePropertiesComponent.h"

UHMGridPropertiesComponent::UHMGridPropertiesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHMGridPropertiesComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHMGridPropertiesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHMGridPropertiesComponent::SetupBaseProperties()
{
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	TileSize = Grid->TileSize;
	TileHeight = Grid->TileHeight;
}

void UHMGridPropertiesComponent::SetupTilesProperties()
{
	TilesLocations.Empty();
	ClearTilesLocations.Empty();

	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	TArray<AHMTile*> Tiles = Grid->Tiles;

	for (AHMTile* Tile : Tiles)
	{
		if (Tile->bAttached)
		{
			TilesLocations.Add(Tile->GetActorLocation());
			UHMTilePropertiesComponent* TilePropertiesComponent = Tile->FindComponentByClass<UHMTilePropertiesComponent>();
			if (TilePropertiesComponent && TilePropertiesComponent->bIsClear)
			{
				ClearTilesLocations.Add(Tile->GetActorLocation());
			}
			else if (!TilePropertiesComponent)
			{
				UE_LOG(LogTemp, Warning, TEXT("Tile doesn't contain properties component!"));
			}
		}
		else
		{
			Tile->SetActorHiddenInGame(true);
			UE_LOG(LogTemp, Error, TEXT("Tile doesn't snapped to grid!"));
		}
	}
}

void UHMGridPropertiesComponent::SetupTilesNeighbours()
{
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	TArray<AHMTile*> Tiles = Grid->Tiles;

	for (AHMTile* Tile : Tiles)
	{
		if (Tile->bAttached)
		{
			Tile->Neighbours.Empty();
			FHMCoord HexCoord = Tile->UUID.HexCoord;
			for (FHMCoord Direction : FHMCoord::HexDirections)
			{
				FHMCoord NeighbourTileHexCoord = HexCoord;
				NeighbourTileHexCoord.Q += Direction.Q;
				NeighbourTileHexCoord.R += Direction.R;
				NeighbourTileHexCoord.S += Direction.S;

				AHMTile** NeighbourTile = Grid->TilesToLocationsLinkages.Find(NeighbourTileHexCoord.ToVec());
				if (NeighbourTile)
				{
					Tile->Neighbours.Add((*NeighbourTile));
				}
			}
			if (Tile->Neighbours.Num() > 6)
			{
				UE_LOG(LogTemp, Error, TEXT("Wrong number of Neighbours attached to Tile!"));
			}
		}
	}
}

void UHMGridPropertiesComponent::GetTileSnapLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, FVector& OutLocation, bool& bIsClear)
{
	float Distance = std::numeric_limits<float>::max();
	for (int i = 0; i < TilesLocations.Num(); ++i)
	{
		FVector SnapLocation = TilesLocations[i];
		FVector GoalLocation = CurrentLocation;
		SnapLocation.Z = 0.f;
		GoalLocation.Z = 0.f;

		float CurrentDistance = FVector::Distance(GoalLocation, SnapLocation);
		if (CurrentDistance < Distance)
		{
			Distance = CurrentDistance;
			OutLocation = TilesLocations[i];
		}
	}
	bIsClear = SourceLocations.Contains(OutLocation);
}

void UHMGridPropertiesComponent::GetTileSnapLocations(const TArray<FVector>& SourceLocations, int32 ReqLocationsNum, const FVector& CurrentLocation, TArray<FVector>& OutClearLocations, TArray<FVector>& OutUnClearLocations, FVector& OutCenterLocation)
{
	bool bIsClear = false;
	GetTileSnapLocation(SourceLocations, CurrentLocation, OutCenterLocation, bIsClear);

	if (bIsClear)
	{
		OutClearLocations.Add(OutCenterLocation);
	}
	else
	{
		OutUnClearLocations.Add(OutCenterLocation);
	}

	if (ReqLocationsNum == 1)
	{
		return;
	}

	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	FHMCoord HexCoord = FHMUtilities::ToHex(GetWorld(), OutCenterLocation);
	AHMTile** Tile = Grid->TilesToLocationsLinkages.Find(HexCoord.ToVec());
	if (Tile)
	{
		for (AHMTile* NeighbourTile : (*Tile)->Neighbours)
		{
			FVector TileLocation = NeighbourTile->GetActorLocation();
			if (FMath::Abs(OutCenterLocation.Z - TileLocation.Z) > MinPossibleDifferential)
			{
				OutUnClearLocations.Add(TileLocation);
			}
			else
			{
				if (SourceLocations.Contains(TileLocation))
				{
					OutClearLocations.Add(TileLocation);
				}
				else
				{
					OutUnClearLocations.Add(TileLocation);
				}
			}
		}
	}

	while (ReqLocationsNum > OutClearLocations.Num() + OutUnClearLocations.Num())
	{
		OutUnClearLocations.Add(FVector::ZeroVector);
	}

	if (ReqLocationsNum < OutClearLocations.Num() + OutUnClearLocations.Num())
	{
		TArray<std::pair<bool, FVector>> OutLocations;
		for (FVector Location : OutClearLocations)
		{
			OutLocations.Add(std::make_pair(true, Location));
		}
		for (FVector Location : OutUnClearLocations)
		{
			OutLocations.Add(std::make_pair(false, Location));
		}
		OutLocations.Sort([OutCenterLocation](const std::pair<bool, FVector>& Value1, const std::pair<bool, FVector>& Value2) {
			return FVector::Distance(OutCenterLocation, Value1.second) < FVector::Distance(OutCenterLocation, Value2.second);
		});
		OutClearLocations.Empty();
		OutUnClearLocations.Empty();
		int32 Index = 0;
		for (const std::pair<bool, FVector>& Location : OutLocations)
		{
			Index++;
			if (Location.first)
			{
				OutClearLocations.Add(Location.second);
			}
			else
			{
				OutUnClearLocations.Add(Location.second);
			}
			if (Index == ReqLocationsNum)
			{
				break;
			}
		}
	}
}

void UHMGridPropertiesComponent::GetTileSnapClearLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, FVector& OutLocation, float& OutDistance)
{
	OutDistance = std::numeric_limits<float>::max();
	for (int i = 0; i < SourceLocations.Num(); ++i)
	{
		FVector SnapLocation = SourceLocations[i];
		FVector GoalLocation = CurrentLocation;
		SnapLocation.Z = 0.f;
		GoalLocation.Z = 0.f;

		float CurrentDistance = FVector::Distance(GoalLocation, SnapLocation);
		if (CurrentDistance < OutDistance)
		{
			OutDistance = CurrentDistance;
			OutLocation = SourceLocations[i];
		}
	}
}

void UHMGridPropertiesComponent::GetTileSnapClearLocations(const TArray<FVector>& SourceLocations, int32 ReqLocationsNum, bool bIgnoreHeight, const FVector& CurrentLocation, TArray<FVector>& OutLocations, FVector& OutCenterLocation, float& OutDistanceToCenter)
{
	if (ReqLocationsNum == 1)
	{
		GetTileSnapClearLocation(SourceLocations, CurrentLocation, OutCenterLocation, OutDistanceToCenter);
		OutLocations.Add(OutCenterLocation);
		return;
	}

	OutDistanceToCenter = std::numeric_limits<float>::max();
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	AHMTile* NearestTile = nullptr;
	TArray<AHMTile*> ValidatedNeighbours;

	float MinDistance = std::numeric_limits<float>::max();
	for (int i = 0; i < SourceLocations.Num(); ++i)
	{
		FVector SnapLocation = SourceLocations[i];
		FHMCoord HexCoord = FHMUtilities::ToHex(GetWorld(), SnapLocation);
		AHMTile** Tile = Grid->TilesToLocationsLinkages.Find(HexCoord.ToVec());
		if (Tile)
		{
			TArray<AHMTile*> InvalidatedNeighbours = (*Tile)->Neighbours;
			InvalidatedNeighbours.RemoveAll([SourceLocations](const AHMTile* NeighbourTile) {
				return !SourceLocations.Contains(NeighbourTile->GetActorLocation());
			});
			if (InvalidatedNeighbours.Num() >= ReqLocationsNum - 1)
			{
				if (!bIgnoreHeight)
				{
					bool bIsHeightSame = true;
					float TileLocationZ = (*Tile)->GetActorLocation().Z;
					for (AHMTile* NeighbourTile : InvalidatedNeighbours)
					{
						if (FMath::Abs(TileLocationZ - NeighbourTile->GetActorLocation().Z) > MinPossibleDifferential)
						{
							bIsHeightSame = false;
							break;
						}
					}
					if (!bIsHeightSame)
					{
						continue;
					}
				}
				FVector GoalLocation = CurrentLocation;
				SnapLocation.Z = 0.f;
				GoalLocation.Z = 0.f;

				float CurrentDistance = FVector::Distance(GoalLocation, SnapLocation);
				if (CurrentDistance < MinDistance)
				{
					MinDistance = CurrentDistance;
					NearestTile = (*Tile);
					ValidatedNeighbours = InvalidatedNeighbours;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find Tile attached to location!"));
			continue;
		}
	}
	if (NearestTile)
	{
		OutLocations.Add(NearestTile->GetActorLocation());
		TMap<FVector, float> Distances;
		for (AHMTile* NeighbourTile : ValidatedNeighbours)
		{
			Distances.Add(NeighbourTile->GetActorLocation(), FVector::Distance(CurrentLocation, NeighbourTile->GetActorLocation()));
		}
		Distances.ValueSort([](const float& Value1, const float& Value2) {
			return Value1 < Value2;
		});
		FVector MinBound = FVector(OutLocations[0].X, OutLocations[0].Y, 0.f);
		FVector MaxBound = FVector(OutLocations[0].X, OutLocations[0].Y, 0.f);;
		for (auto& Element : Distances)
		{
			OutLocations.Add(Element.Key);
			if (Element.Key.X < MinBound.X)
			{
				MinBound.X = Element.Key.X;
			}
			if (Element.Key.Y < MinBound.Y)
			{
				MinBound.Y = Element.Key.Y;
			}
			if (Element.Key.X > MaxBound.X)
			{
				MaxBound.X = Element.Key.X;
			}
			if (Element.Key.Y > MaxBound.Y)
			{
				MaxBound.Y = Element.Key.Y;
			}
			if (OutLocations.Num() == ReqLocationsNum)
			{
				break;
			}
		}

		OutCenterLocation.X = MinBound.X + (MaxBound.X - MinBound.X) *.5f;
		OutCenterLocation.Y = MinBound.Y + (MaxBound.Y - MinBound.Y) *.5f;
		OutCenterLocation.Z = OutLocations[0].Z;
		OutDistanceToCenter = FVector::Distance(FVector(OutCenterLocation.X, OutCenterLocation.Y, 0.f), FVector(CurrentLocation.X, CurrentLocation.Y, 0.f));
	}
}

AHMTile* UHMGridPropertiesComponent::GetTileByLocation(const FVector& Location)
{
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	FHMCoord HexCoord = FHMUtilities::ToHex(GetWorld(), Location);
	AHMTile** Tile = Grid->TilesToLocationsLinkages.Find(HexCoord.ToVec());
	if (Tile)
	{
		return (*Tile);
	}
	return nullptr;
}

TArray<AHMTile*> UHMGridPropertiesComponent::GetTilesByLocationInRange(const FVector& Location, int32 Radius)
{
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	FVector SnapLocation = FHMUtilities::ToSnapLocation(GetWorld(), Location);
	TArray<AHMTile*> Tiles;

	for (int32 Q = -Radius; Q <= Radius; ++Q)
	{
		int32 R1 = std::max(-Radius, -Q - Radius);
		int32 R2 = std::min(Radius, -Q + Radius);

		for (int32 R = R1; R <= R2; R++)
		{
			FHMCoord HexCoord = FHMCoord::Init(Q, R, -Q - R);
			FVector2D Location2D = FHMCoord::ToLocation(Grid->Layout, HexCoord);

			AHMTile* Tile = GetTileByLocation(FVector(Location2D.X + SnapLocation.X, Location2D.Y + SnapLocation.Y, 0.f));
			if (Tile)
			{
				Tiles.Add(Tile);
			}
		}
	}
	return Tiles;
}

