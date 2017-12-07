// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "HMCoord.h"
#include "UObject/NoExportTypes.h"
#include "HMUtilities.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FHMTileUUID
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	FHMCoord HexCoord;

	static FHMTileUUID Init(const FHMCoord& HexCoord_);
	static bool Compare(const FHMTileUUID& UUID1, const FHMTileUUID& UUID2);
	static FHMTileUUID Undefined();
	bool IsUndefined();
};

class HEXMAP_API FHMUtilities
{
public:

	static class AHMGrid* GetGrid(UWorld* World, bool& OutError);
	static class AHMGrid* GetGrid(UWorld* World);

	static FHMCoord ToHex(UWorld* World, const FVector& Location);
	static FVector ToSnapLocation(UWorld* World, const FVector& Location);
	static FVector ToSnapLocation(UWorld* World, const FHMCoord& HexCoord);

	static FHMCoord ToNearestHex(UWorld* World, const FVector& Location);
	static FVector ToNearestSnapLocation(UWorld* World, const FVector& Location, bool bZIncludeTileHeight = false);
};