// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "HMMeshComponent.generated.h"

USTRUCT()
struct FHMMeshVertex
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector Position;

	UPROPERTY()
	FColor Color;

	UPROPERTY()
	float U;

	UPROPERTY()
	float V;
};

USTRUCT()
struct FHMMeshTriangle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FHMMeshVertex> Vertices;
};

UCLASS(hidecategories = (Object, LOD, Physics, Collision), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class HEXMAP_API UHMMeshComponent : public UMeshComponent
{
	GENERATED_UCLASS_BODY()

private:

	friend class FHMMeshSceneProxy;
	TArray<FHMMeshTriangle> Triangles;

protected:

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual int32 GetNumMaterials() const override;
	virtual FBoxSphereBounds CalcBounds(const FTransform & LocalToWorld) const override;

public:

	static const int32 NumSubdivisions;

	void SetTriangles(const TArray<FHMMeshTriangle>& Triangles);
	void AddTriangles(const TArray<FHMMeshTriangle>& Triangles);
	void ClearTriangles();

	static void AddHexTileGeometry(const FVector2D& Location2D, float Size, TArray<struct FHMMeshTriangle>& Triangles);
};