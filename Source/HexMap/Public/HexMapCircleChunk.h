// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexMapChunk.h"
#include "HexMapCircleChunk.generated.h"

/**
 * 
 */
UCLASS()
class HEXMAP_API AHexMapCircleChunk : public AHexMapChunk
{
	GENERATED_BODY()

private:

protected:

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;

#endif

	virtual void GenerateTilesMetadata(bool bIsReconstruct) override;
	virtual void GenerateTilesGeometry(bool bIsReconstruct) override;

public:

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 8))
	int32 Radius = 2;

#if WITH_EDITOR

	void SetRadius(int32 Radius);

#endif
};
