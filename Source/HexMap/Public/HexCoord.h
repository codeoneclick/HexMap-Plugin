// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <vector>
#include "HexCoord.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FHexCoord
{
	GENERATED_USTRUCT_BODY()

public:

	UENUM(BlueprintType)
	enum class EHexDirection : int8
	{
		ODD UMETA(DisplayName = "Odd") = -1,
		EVEN UMETA(DisplayName = "Even") = 1
	};

	UPROPERTY(VisibleAnywhere)
	int32 Q;

	UPROPERTY(VisibleAnywhere)
	int32 R;

	UPROPERTY(VisibleAnywhere)
	int32 S;

	static const std::vector<FHexCoord> HexDirections;

	static FHexCoord Init(int32 Q_, int32 R_, int32 S_);

	static FHexCoord Round(const struct FFractionalHexCoord& FractionalHexCoord);
	static bool Compare(const FHexCoord& HexCoord1, const FHexCoord& HexCoord2);

	static FVector2D QOffsetFromCube(EHexDirection Direction, const FHexCoord& HexCoord);
	static FHexCoord QOffsetToCube(EHexDirection Direction, const FVector2D& Location);
	static FVector2D ROffsetFromCube(EHexDirection Direction, const FHexCoord& HexCoord);
	static FHexCoord ROffsetToCube(EHexDirection Direction, const FVector2D& Location);

	static FVector2D ToLocation(const struct FHexLayout& Layout, const FHexCoord& HexCoord);
	static struct FFractionalHexCoord ToHex(const struct FHexLayout& Layout, const FVector2D& Location);

	FIntVector ToVec() const;
};

USTRUCT(Blueprintable, BlueprintType)
struct FFractionalHexCoord
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	float Q;

	UPROPERTY(VisibleAnywhere)
	float R;

	UPROPERTY(VisibleAnywhere)
	float S;

	static FFractionalHexCoord Init(float Q_, float R_, float S_);
};

USTRUCT(Blueprintable, BlueprintType)
struct FHexOrientation
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	float F0;

	UPROPERTY(VisibleAnywhere)
	float F1;

	UPROPERTY(VisibleAnywhere)
	float F2;

	UPROPERTY(VisibleAnywhere)
	float F3;

	UPROPERTY(VisibleAnywhere)
	float B0;

	UPROPERTY(VisibleAnywhere)
	float B1;

	UPROPERTY(VisibleAnywhere)
	float B2;

	UPROPERTY(VisibleAnywhere)
	float B3;

	UPROPERTY(VisibleAnywhere)
	float StartAngle;

	static FHexOrientation Init(float F0_, float F1_, float F2_, float F3_, float B0_, float B1_, float B2_, float B3_, float StartAngle_);
};

USTRUCT(Blueprintable, BlueprintType)
struct FHexLayout
{
	GENERATED_USTRUCT_BODY()

public:

	static const FHexOrientation Pointy;
	static const FHexOrientation Flat;

	UPROPERTY(VisibleAnywhere)
	FHexOrientation Orientation;

	UPROPERTY(VisibleAnywhere)
	FVector2D Size;

	UPROPERTY(VisibleAnywhere)
	FVector2D Origin;

	static FHexLayout Init(const FHexOrientation& Orientation_, const FVector2D& Size_, const FVector2D& Origin_);
};
