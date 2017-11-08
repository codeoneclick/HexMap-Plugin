
#pragma once

#include "CoreMinimal.h"
#include "HexCoord.h"
#include "HexUtils.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FTileUUID
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 ChunkIndex;

	UPROPERTY(VisibleAnywhere)
	FHexCoord HexCoord;

	static FTileUUID Init(int32 _ChunkIndex, const FHexCoord& _HexCoord);
	static bool Compare(const FTileUUID& UUID1, const FTileUUID& UUID2);
	static FTileUUID Undefined();
	bool IsUndefined();
};

class HEXMAP_API FHex
{
public:

	FHex();
	~FHex();

	static class AHexMapGrid* GetGrid(UWorld* World, bool& OutError);
	static class AHexMapGrid* GetGrid(UWorld* World);

	static FVector GetTileLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation, float& OutDistance);
	static FVector GetTileLocation(const TArray<FVector>& SourceLocations, const FVector& CurrentLocation);
};