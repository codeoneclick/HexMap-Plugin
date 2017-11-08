// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HexUtils.h"
#include "HexMapTileLocationComponent.generated.h"

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
