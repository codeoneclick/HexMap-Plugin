
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMCoord.h"
#include "HMGrid.h"
#include "HMTile.h"
#include "HMUtilities.h"

FHMTileUUID FHMTileUUID::Init(const FHMCoord& HexCoord_)
{
	FHMTileUUID UUID;
	UUID.HexCoord = HexCoord_;
	return UUID;
}

bool FHMTileUUID::Compare(const FHMTileUUID& UUID1, const FHMTileUUID& UUID2)
{
	return  FHMCoord::Compare(UUID1.HexCoord, UUID2.HexCoord);
}

FHMTileUUID FHMTileUUID::Undefined()
{
	FHMTileUUID Id;
	Id.HexCoord = FHMCoord::Init(std::numeric_limits<int32>::max(), std::numeric_limits<int32>::max(), std::numeric_limits<int32>::max());
	return Id;
};

bool FHMTileUUID::IsUndefined()
{
	return HexCoord.Q == std::numeric_limits<int32>::max() && HexCoord.R == std::numeric_limits<int32>::max() && HexCoord.S == std::numeric_limits<int32>::max();
}

AHMGrid* FHMUtilities::GetGrid(UWorld* World, bool& OutError)
{
	OutError = false;
	AHMGrid* Grid = nullptr;
	int32 InstancesCount = 0;
	for (TActorIterator<AHMGrid> It(World); It; ++It)
	{
		Grid = *It;
		InstancesCount++;
	}
	if (InstancesCount > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Instances Count for HexMapGrid!"));
		OutError = true;
		return nullptr;
	}
	return Grid;
}

AHMGrid* FHMUtilities::GetGrid(UWorld* World)
{
	bool bError = false;
	return GetGrid(World, bError);
}

FHMCoord FHMUtilities::ToHex(UWorld* World, const FVector& Location)
{
	AHMGrid* Grid = FHMUtilities::GetGrid(World);
	FHMFractionalCoord FractionalCoord = FHMCoord::ToHex(Grid->Layout, FVector2D(Location.X, Location.Y));
	return FHMCoord::Round(FractionalCoord);
}

FVector FHMUtilities::ToSnapLocation(UWorld* World, const FVector& Location)
{
	AHMGrid* Grid = FHMUtilities::GetGrid(World);
	FVector2D Location2D = FHMCoord::ToLocation(Grid->Layout, ToHex(World, Location));
	return FVector(Location2D.X, Location2D.Y, Location.Z);
}

FVector FHMUtilities::ToSnapLocation(UWorld* World, const FHMCoord& HexCoord)
{
	AHMGrid* Grid = FHMUtilities::GetGrid(World);
	FVector2D Location2D = FHMCoord::ToLocation(Grid->Layout, HexCoord);
	return FVector(Location2D.X, Location2D.Y, 0.f);
}


