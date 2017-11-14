// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "HMMeshComponent.generated.h"

USTRUCT()
struct FHMMeshVertex
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Triangle)
	FVector Position;

	UPROPERTY(EditAnywhere, Category = Triangle)
	FColor Color;

	UPROPERTY(EditAnywhere, Category = Triangle)
	float U;

	UPROPERTY(EditAnywhere, Category = Triangle)
	float V;
};

USTRUCT()
struct FHMMeshTriangle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Triangle)
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