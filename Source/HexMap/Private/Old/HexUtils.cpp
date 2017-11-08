#include "../HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapGrid.h"
#include "HexUtils.h"

FTileUUID FTileUUID::Init(int32 _ChunkIndex, const FHexCoord& _HexCoord)
{
	FTileUUID UUID;
	UUID.ChunkIndex = _ChunkIndex;
	UUID.HexCoord = _HexCoord;
	return UUID;
}

bool FTileUUID::Compare(const FTileUUID& UUID1, const FTileUUID& UUID2)
{
	return UUID1.ChunkIndex == UUID2.ChunkIndex && FHexCoord::Compare(UUID1.HexCoord, UUID2.HexCoord);
}

FTileUUID FTileUUID::Undefined()
{
	FTileUUID Id;
	Id.ChunkIndex = std::numeric_limits<int32>::max();
	Id.HexCoord = FHexCoord::Init(std::numeric_limits<int32>::max(), std::numeric_limits<int32>::max(), std::numeric_limits<int32>::max());
	return Id;
};

bool FTileUUID::IsUndefined()
{
	return ChunkIndex == std::numeric_limits<int32>::max() && HexCoord.Q == std::numeric_limits<int32>::max() && HexCoord.R == std::numeric_limits<int32>::max() && HexCoord.S == std::numeric_limits<int32>::max();
}

FHex::FHex()
{

}

FHex::~FHex()
{

}

AHexMapGrid* FHex::GetGrid(UWorld* World, bool& OutError)
{
	OutError = false;
	AHexMapGrid* Grid = nullptr;
	int32 InstancesCount = 0;
	for (TActorIterator<AHexMapGrid> It(World); It; ++It)
	{
		Grid = *It;
		InstancesCount++;
	}
	if (InstancesCount > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("[HexMapGrid] Wrong Instances Count!"));
		OutError = true;
		return nullptr;
	}
	return Grid;
}

AHexMapGrid* FHex::GetGrid(UWorld* World)
{
	bool bError = false;
	return GetGrid(World, bError);
}

FVector FHex::GetTileLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, float& OutDistance)
{
	FVector HexMapTileLocation = FVector::ZeroVector;
	OutDistance = std::numeric_limits<float>::max();
	FVector2D SnapLocation;
	FVector2D GoalLocation = FVector2D(CurrentLocation.X, CurrentLocation.Y);
	for (int32 i = 0; i < SourceLocations.Num(); ++i)
	{
		SnapLocation.X = SourceLocations[i].X;
		SnapLocation.Y = SourceLocations[i].Y;

		float CurrentDistance = FVector2D::Distance(GoalLocation, SnapLocation);
		if (CurrentDistance < OutDistance)
		{
			OutDistance = CurrentDistance;
			HexMapTileLocation = SourceLocations[i];
		}
	}
	return HexMapTileLocation;
}

FVector FHex::GetTileLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation)
{
	float OutDistance = 0.f;
	return GetTileLocation(SourceLocations, CurrentLocation, OutDistance);
}