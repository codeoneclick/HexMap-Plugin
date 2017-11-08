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

UCLASS()
class HEXMAP_API UHMMeshComponent : public UMeshComponent, public IInterface_CollisionDataProvider
{
	GENERATED_BODY()
private:

	friend class FHMMeshSceneProxy;
	TArray<FHMMeshTriangle> Triangles;

protected:

	virtual FBoxSphereBounds CalcBounds(const FTransform & LocalToWorld) const override;

public:

	void SetTriangles(const TArray<FHMMeshTriangle>& Triangles);
	void AddTriangles(const TArray<FHMMeshTriangle>& Triangles);
	void ClearTriangles();

	virtual bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData) override;
	virtual bool ContainsPhysicsTriMeshData(bool InUseAllTriData) const override;
	virtual bool WantsNegXTriMesh() override { return false; }

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual class UBodySetup* GetBodySetup() override;

	virtual int32 GetNumMaterials() const override;

	void UpdateBodySetup();
	void UpdateCollision();

	UPROPERTY(EditAnywhere)
	class UBodySetup* ModelBodySetup;
};