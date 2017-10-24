// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexMapChunk.h"
#include "HexMapRectangleChunk.generated.h"

UCLASS()
class HEXMAP_API AHexMapRectangleChunk : public AHexMapChunk
{
	GENERATED_BODY()

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;

#endif

	virtual void GenerateTilesMetadata(bool bIsReconstruct) override;
	virtual void GenerateTilesGeometry(bool bIsReconstruct) override;

public:	

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 16))
	int32 SizeX = 4;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 16))
	int32 SizeY = 4;

#if WITH_EDITOR

	void SetSizeX(int32 SizeX);
	void SetSizeY(int32 SizeY);
	void SetSize(int32 SizeX, int32 SizeY);

#endif

};
