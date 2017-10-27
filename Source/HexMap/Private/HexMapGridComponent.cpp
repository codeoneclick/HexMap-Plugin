// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapGridComponent.h"
#include "HexMapGrid.h"
#include "HexMapChunk.h"
#include "HexMapTile.h"
#include "HexCoord.h"
#include "HexMapTileLocationComponent.h"
#include "HexNavigation.h"

UHexMapGridComponent::UHexMapGridComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHexMapGridComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHexMapGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHexMapGridComponent::SetupBaseProperties()
{
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	TileSize = Grid->TileSize;
	TileHeight = Grid->TileHeight;
}

void UHexMapGridComponent::SetupTilesProperties()
{
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());

	TilesLocations.Empty();
	ClearTilesLocations.Empty();

	TArray<AHexMapChunk*> Chunks = Grid->Chunks;
	for (AHexMapChunk* Chunk : Chunks)
	{
		TArray<USceneComponent*> ComponentsInRoot;
		Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
		for (USceneComponent* ComponentInRoot : ComponentsInRoot)
		{
			if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
			{
				UHexMapTileLocationComponent* TileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
				FVector Location = ComponentInRoot->GetComponentLocation();
				if (TileLocationComponent->LinkedTile)
				{
					TilesLocations.Add(Location);
					bool bIsClear = TileLocationComponent->LinkedTile->bIsClear;
					if (bIsClear)
					{
						ClearTilesLocations.Add(Location);
					}
				}
			}
		}
	}
}

void UHexMapGridComponent::SetupTilesNeighbours()
{
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	TArray<AHexMapChunk*> Chunks = Grid->Chunks;
	for (AHexMapChunk* Chunk : Chunks)
	{
		TArray<USceneComponent*> ComponentsInRoot;
		Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
		for (USceneComponent* ComponentInRoot : ComponentsInRoot)
		{
			if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
			{
				UHexMapTileLocationComponent* TileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
				if (!TileLocationComponent->LinkedTile)
				{
					TileLocationComponent->Neighbours.Empty();
				}
				else
				{
					TileLocationComponent->Neighbours.RemoveAll([](const UHexMapTileLocationComponent* TileLocationComponent) {
						return !TileLocationComponent->LinkedTile;
					});
				}
				if (TileLocationComponent->Neighbours.Num() > 6)
				{
					UE_LOG(LogTemp, Error, TEXT("Wrong number of Neighbours attached to Tile!"));
				}
			}
		}
	}
}

void UHexMapGridComponent::GetHexGridSnapLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, FVector& OutLocation, bool& bIsClear)
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

void UHexMapGridComponent::GetHexGridSnapLocations(const TArray<FVector>& SourceLocations, int32 ReqLocationsNum, const FVector& CurrentLocation, TArray<FVector>& OutClearLocations, TArray<FVector>& OutUnClearLocations, FVector& OutCenterLocation)
{
	bool bIsClear = false;
	GetHexGridSnapLocation(SourceLocations, CurrentLocation, OutCenterLocation, bIsClear);

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

	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	AHexMapChunk** ChunkIt = Grid->ChunksToLocationsLinkages.Find(FVector2D(OutCenterLocation.X, OutCenterLocation.Y));
	if (ChunkIt)
	{
		AHexMapChunk* Chunk = *ChunkIt;
		UHexMapTileLocationComponent** HexMapTileLocationComponentIt = Chunk->TileLocationComponentsToLocationsLinkages.Find(FVector2D(OutCenterLocation.X, OutCenterLocation.Y));
		if (HexMapTileLocationComponentIt)
		{
			UHexMapTileLocationComponent* HexMapTileLocationComponent = *HexMapTileLocationComponentIt;
			float TileHeight = OutCenterLocation.Z;
			for (UHexMapTileLocationComponent* HexMapTileLocationComponent : HexMapTileLocationComponent->Neighbours)
			{
				FVector TileLocation = HexMapTileLocationComponent->GetComponentLocation();
				if (FMath::Abs(TileHeight - TileLocation.Z) > TileHeightTrashhold)
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

void UHexMapGridComponent::GetHexGridSnapClearLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, FVector& OutLocation, float& OutDistance)
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

void UHexMapGridComponent::GetHexGridSnapClearLocations(const TArray<FVector>& SourceLocations, int32 ReqLocationsNum, bool bIgnoreHeight, const FVector& CurrentLocation, TArray<FVector>& OutLocations, FVector& OutCenterLocation, float& OutDistanceToCenter)
{
	if (ReqLocationsNum == 1)
	{
		GetHexGridSnapClearLocation(SourceLocations, CurrentLocation, OutCenterLocation, OutDistanceToCenter);
		OutLocations.Add(OutCenterLocation);
		return;
	}

	OutDistanceToCenter = std::numeric_limits<float>::max();
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	UHexMapTileLocationComponent* NearestTileLocationComponent = nullptr;
	TArray<UHexMapTileLocationComponent*> ValidatedNeighbours;

	float MinDistance = std::numeric_limits<float>::max();
	for (int i = 0; i < SourceLocations.Num(); ++i)
	{
		FVector SnapLocation = SourceLocations[i];
		AHexMapChunk** ChunkIt = Grid->ChunksToLocationsLinkages.Find(FVector2D(SnapLocation.X, SnapLocation.Y));
		if (ChunkIt)
		{
			AHexMapChunk* Chunk = *ChunkIt;
			UHexMapTileLocationComponent** HexMapTileLocationComponentIt = Chunk->TileLocationComponentsToLocationsLinkages.Find(FVector2D(SnapLocation.X, SnapLocation.Y));
			if (HexMapTileLocationComponentIt)
			{
				UHexMapTileLocationComponent* HexMapTileLocationComponent = *HexMapTileLocationComponentIt;
				TArray<UHexMapTileLocationComponent*> InvalidatedNeighbours = HexMapTileLocationComponent->Neighbours;
				InvalidatedNeighbours.RemoveAll([SourceLocations](const UHexMapTileLocationComponent* Neighbour) {
					return !SourceLocations.Contains(Neighbour->GetComponentLocation());
				});
				if (InvalidatedNeighbours.Num() >= ReqLocationsNum - 1)
				{
					if (!bIgnoreHeight)
					{
						bool bIsHeightSame = true;
						float TileHeight = HexMapTileLocationComponent->GetComponentLocation().Z;
						for (UHexMapTileLocationComponent* HexMapTileLocationComponent : InvalidatedNeighbours)
						{
							if (FMath::Abs(TileHeight - HexMapTileLocationComponent->GetComponentLocation().Z) > TileHeightTrashhold)
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
						NearestTileLocationComponent = HexMapTileLocationComponent;
						ValidatedNeighbours = InvalidatedNeighbours;
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Can't find HexMapTileLocationComponent attached to Location!"));
				continue;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find HexMapChunk attached to Location!"));
			continue;
		}
	}
	if (NearestTileLocationComponent)
	{
		OutLocations.Add(NearestTileLocationComponent->GetComponentLocation());
		TMap<FVector, float> Distances;
		for (UHexMapTileLocationComponent* HexMapTileLocationComponent : ValidatedNeighbours)
		{
			Distances.Add(HexMapTileLocationComponent->GetComponentLocation(), FVector::Distance(CurrentLocation, HexMapTileLocationComponent->GetComponentLocation()));
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

const TArray<FVector>& UHexMapGridComponent::GetClearTilesLocations() const
{
	return ClearTilesLocations;
}

FVector UHexMapGridComponent::GetTileLocation(const FVector& Location) const
{
	return FHex::GetTileLocation(TilesLocations, Location);
}

AHexMapTile* UHexMapGridComponent::GetTileByLocation(const FVector& Location) const
{
	FVector CenterLocation = GetTileLocation(Location);
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());
	AHexMapChunk** ChunkIt = Grid->ChunksToLocationsLinkages.Find(FVector2D(CenterLocation.X, CenterLocation.Y));
	if (ChunkIt)
	{
		AHexMapChunk* Chunk = *ChunkIt;
		UHexMapTileLocationComponent** TileLocationComponentIt = Chunk->TileLocationComponentsToLocationsLinkages.Find(FVector2D(CenterLocation.X, CenterLocation.Y));
		if (TileLocationComponentIt)
		{
			UHexMapTileLocationComponent* TileLocationComponent = *TileLocationComponentIt;
			return TileLocationComponent->LinkedTile;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find HexMapTileLocationComponent assigned to Location!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find HexMapChunk assigned to Location!"));
	}
	return nullptr;
}

TArray<AHexMapTile*> UHexMapGridComponent::GetTilesByLocationInRange(const FVector& Location, int Radius)
{
	FVector CenterLocation = GetTileLocation(Location);
	TArray<AHexMapTile*> Tiles;
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld());

	for (int32 Q = -Radius; Q <= Radius; ++Q)
	{
		int32 R1 = std::max(-Radius, -Q - Radius);
		int32 R2 = std::min(Radius, -Q + Radius);

		for (int32 R = R1; R <= R2; R++)
		{
			FHexCoord HexLocation = FHexCoord::Init(Q, R, -Q - R);
			FVector2D Location2D = FHexCoord::ToLocation(Grid->Layout, HexLocation);

			AHexMapTile* Tile = GetTileByLocation(FVector(Location2D.X + CenterLocation.X, Location2D.Y + CenterLocation.Y, 0.f));
			if (Tile)
			{
				Tiles.Add(Tile);
			}
		}
	}
	return Tiles;
}