// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HexUtils.h"
#include "HexMapTileLocationComponent.generated.h"

/*USTRUCT(Blueprintable, BlueprintType)
struct FTileUUID
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 C;

	UPROPERTY(VisibleAnywhere)
	int32 Q;

	UPROPERTY(VisibleAnywhere)
	int32 R;

	UPROPERTY(VisibleAnywhere)
	int32 S;

	static bool Compare(const FTileUUID& UUID1, const FTileUUID& UUID2)
	{
		return UUID1.C == UUID2.C && UUID1.Q == UUID2.Q && UUID1.R == UUID2.R && UUID1.S == UUID2.S;
	}

	static FTileUUID Undefined()
	{
		FTileUUID Id;
		Id.C = std::numeric_limits<int32>::max();
		Id.Q = std::numeric_limits<int32>::max();
		Id.R = std::numeric_limits<int32>::max();
		Id.S = std::numeric_limits<int32>::max();
		return Id;
	};

	bool IsUndefined()
	{
		return C == std::numeric_limits<int32>::max() && Q == std::numeric_limits<int32>::max() && R == std::numeric_limits<int32>::max() && S == std::numeric_limits<int32>::max();
	}
};*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHexMapTileLocationComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UHexMapTileLocationComponent();
	~UHexMapTileLocationComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere)
	class AHexMapTile* LinkedTile = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<class UHexMapTileLocationComponent*> Neighbours;

	UPROPERTY(VisibleAnywhere)
	FTileUUID UUID;

	UPROPERTY(VisibleAnywhere)
	float CachedLocationZ = 0.f;
};
