// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <vector>
#include "HMCoord.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FHMCoord
{
	GENERATED_USTRUCT_BODY()

public:

	UENUM(BlueprintType)
	enum class EHMDirection : int8
	{
		ODD UMETA(DisplayName = "Odd") = -1,
		EVEN UMETA(DisplayName = "Even") = 1
	};

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	int32 Q;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	int32 R;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	int32 S;

	static const std::vector<FHMCoord> HexDirections;

	static FHMCoord Init(int32 Q_, int32 R_, int32 S_);

	static FHMCoord Round(const struct FHMFractionalCoord& FractionalCoord);
	static bool Compare(const FHMCoord& HexCoord1, const FHMCoord& HexCoord2);

	static FVector2D QOffsetFromCube(EHMDirection Direction, const FHMCoord& HexCoord);
	static FHMCoord QOffsetToCube(EHMDirection Direction, const FVector2D& Location);
	static FVector2D ROffsetFromCube(EHMDirection Direction, const FHMCoord& HexCoord);
	static FHMCoord ROffsetToCube(EHMDirection Direction, const FVector2D& Location);

	static FVector2D ToLocation(const struct FHMLayout& Layout, const FHMCoord& HexCoord);
	static struct FHMFractionalCoord ToHex(const struct FHMLayout& Layout, const FVector2D& Location);

	FIntVector ToVec() const;
};

USTRUCT(Blueprintable, BlueprintType)
struct FHMFractionalCoord
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float Q;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float R;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float S;

	static FHMFractionalCoord Init(float Q_, float R_, float S_);
};

USTRUCT(Blueprintable, BlueprintType)
struct FHMOrientation
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float F0;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float F1;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float F2;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float F3;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float B0;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float B1;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float B2;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float B3;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	float StartAngle;

	static FHMOrientation Init(float F0_, float F1_, float F2_, float F3_, float B0_, float B1_, float B2_, float B3_, float StartAngle_);
};

USTRUCT(Blueprintable, BlueprintType)
struct FHMLayout
{
	GENERATED_USTRUCT_BODY()

public:

	static const FHMOrientation Pointy;
	static const FHMOrientation Flat;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	FHMOrientation Orientation;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	FVector2D Size;

	UPROPERTY(Category = "Hex Map", VisibleAnywhere)
	FVector2D Origin;

	static FHMLayout Init(const FHMOrientation& Orientation_, const FVector2D& Size_, const FVector2D& Origin_);
};