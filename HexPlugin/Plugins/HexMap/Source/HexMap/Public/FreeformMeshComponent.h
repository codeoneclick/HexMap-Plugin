// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "FreeformMeshComponent.generated.h"

USTRUCT()
struct FFreeformMeshVertex
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
struct FFreeformMeshTriangle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Triangle)
	TArray<FFreeformMeshVertex> Vertices;
};

UCLASS()
class HEXMAP_API UFreeformMeshComponent : public UMeshComponent, public IInterface_CollisionDataProvider
{
	GENERATED_BODY()
private:

	friend class FFreeformMeshSceneProxy;
	TArray<FFreeformMeshTriangle> Triangles;

protected:

	virtual FBoxSphereBounds CalcBounds(const FTransform & LocalToWorld) const override;

public:

	void SetTriangles(const TArray<FFreeformMeshTriangle>& Triangles);
	void AddTriangles(const TArray<FFreeformMeshTriangle>& Triangles);
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
