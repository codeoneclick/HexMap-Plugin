
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "DynamicMeshBuilder.h"
#include "PhysicsEngine/BodySetup.h"
#include "HMMeshComponent.h"

class FHMVBO : public FVertexBuffer
{
public:

	TArray<FDynamicMeshVertex> Vertices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		VertexBufferRHI = RHICreateVertexBuffer(Vertices.Num() * sizeof(FDynamicMeshVertex), BUF_Static, CreateInfo);
		void* Buffer = RHILockVertexBuffer(VertexBufferRHI, 0, Vertices.Num() * sizeof(FDynamicMeshVertex), RLM_WriteOnly);
		FMemory::Memcpy(Buffer, Vertices.GetData(), Vertices.Num() * sizeof(FDynamicMeshVertex));
		RHIUnlockVertexBuffer(VertexBufferRHI);
	}
};

class FHMIBO : public FIndexBuffer
{
public:

	TArray<int32> Indices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		IndexBufferRHI = RHICreateIndexBuffer(sizeof(int32), Indices.Num() * sizeof(int32), BUF_Static, CreateInfo);
		void* Buffer = RHILockIndexBuffer(IndexBufferRHI, 0, Indices.Num() * sizeof(int32), RLM_WriteOnly);
		FMemory::Memcpy(Buffer, Indices.GetData(), Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(IndexBufferRHI);
	}
};

class FHMVertexFactory : public FLocalVertexFactory
{
public:

	void Init(const FHMVBO* VertexBuffer)
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			InitHMVertexFactory,
			FHMVertexFactory*, VertexFactory, this,
			const FHMVBO*, VertexBuffer, VertexBuffer,
			{
				FDataType Declaration;
				Declaration.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Position, VET_Float3);
				Declaration.TextureCoordinates.Add(FVertexStreamComponent(VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate), sizeof(FDynamicMeshVertex), VET_Float2));
				Declaration.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal);
				Declaration.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal);
				Declaration.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Color, VET_Color);
				VertexFactory->SetData(Declaration);
			});
	}
};

class FHMMeshSceneProxy : public FPrimitiveSceneProxy
{
public:

	FHMMeshSceneProxy(UHMMeshComponent* Component) :
		FPrimitiveSceneProxy(Component),
		MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
	{
		for (int32 TriIdx = 0; TriIdx<Component->Triangles.Num(); TriIdx++)
		{
			FHMMeshTriangle& Triangle = Component->Triangles[TriIdx];

			const FVector Edge01 = (Triangle.Vertices[1].Position - Triangle.Vertices[0].Position);
			const FVector Edge02 = (Triangle.Vertices[2].Position - Triangle.Vertices[0].Position);

			const FVector TangentX = Edge01.GetSafeNormal();
			const FVector TangentZ = (Edge02 ^ Edge01).GetSafeNormal();
			const FVector TangentY = (TangentX ^ TangentZ).GetSafeNormal();

			FDynamicMeshVertex Vertex;
			int32 VIndex = 0;
			for (int32 VertexInTriangleIndex = 0; VertexInTriangleIndex < 3; ++VertexInTriangleIndex)
			{
				Vertex.Position = Triangle.Vertices[VertexInTriangleIndex].Position;
				Vertex.Color = Triangle.Vertices[VertexInTriangleIndex].Color;
				Vertex.SetTangents(TangentX, TangentY, TangentZ);
				Vertex.TextureCoordinate.Set(Triangle.Vertices[VertexInTriangleIndex].U, Triangle.Vertices[VertexInTriangleIndex].V);
				VIndex = VertexBuffer.Vertices.Add(Vertex);
				IndexBuffer.Indices.Add(VIndex);
			}
		}

		VertexFactory.Init(&VertexBuffer);

		BeginInitResource(&VertexBuffer);
		BeginInitResource(&IndexBuffer);
		BeginInitResource(&VertexFactory);

		Material = Component->GetMaterial(0);
		if (Material == nullptr)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
	}

	virtual ~FHMMeshSceneProxy()
	{
		VertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_ProceduralMeshSceneProxy_GetDynamicMeshElements);

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy(IsSelected()) : nullptr,
			FLinearColor(0, 0.5f, 1.f)
		);

		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

		FMaterialRenderProxy* MaterialProxy = nullptr;
		if (bWireframe)
		{
			MaterialProxy = WireframeMaterialInstance;
		}
		else
		{
			MaterialProxy = Material->GetRenderProxy(IsSelected());
		}

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];

				FMeshBatch& Mesh = Collector.AllocateMesh();
				FMeshBatchElement& BatchElement = Mesh.Elements[0];
				BatchElement.IndexBuffer = &IndexBuffer;
				Mesh.bWireframe = bWireframe;
				Mesh.VertexFactory = &VertexFactory;
				Mesh.MaterialRenderProxy = MaterialProxy;
				BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());
				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = IndexBuffer.Indices.Num() / 3;
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = VertexBuffer.Vertices.Num() - 1;
				Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
				Mesh.Type = PT_TriangleList;
				Mesh.DepthPriorityGroup = SDPG_World;
				Mesh.bCanApplyViewModeOverrides = false;
				Collector.AddMesh(ViewIndex, Mesh);
			}
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		return Result;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint() const
	{
		return(sizeof(*this) + GetAllocatedSize());
	}

	uint32 GetAllocatedSize(void) const
	{
		return(FPrimitiveSceneProxy::GetAllocatedSize());
	}

private:

	UMaterialInterface* Material;
	FHMVBO VertexBuffer;
	FHMIBO IndexBuffer;
	FHMVertexFactory VertexFactory;

	FMaterialRelevance MaterialRelevance;
};

void UHMMeshComponent::SetTriangles(const TArray<FHMMeshTriangle>& aTriangles)
{
	Triangles = aTriangles;
	UpdateCollision();
	MarkRenderStateDirty();
}

void UHMMeshComponent::AddTriangles(const TArray<FHMMeshTriangle>& aTriangles)
{
	Triangles.Append(aTriangles);
	MarkRenderStateDirty();
}

void UHMMeshComponent::ClearTriangles()
{
	Triangles.Reset();
	MarkRenderStateDirty();
}

FPrimitiveSceneProxy* UHMMeshComponent::CreateSceneProxy()
{
	FPrimitiveSceneProxy* Proxy = nullptr;
	if (Triangles.Num() > 0)
	{
		Proxy = new FHMMeshSceneProxy(this);
	}
	return Proxy;
}

int32 UHMMeshComponent::GetNumMaterials() const
{
	return 1;
}

FBoxSphereBounds UHMMeshComponent::CalcBounds(const FTransform & LocalToWorld) const
{
	if (Triangles.Num() > 0)
	{
		FVector BoundsMin = Triangles[0].Vertices[0].Position;
		FVector BoundsMax = Triangles[0].Vertices[0].Position;

		for (int32 TriangleIndex = 0; TriangleIndex < Triangles.Num(); TriangleIndex++)
		{
			for (int32 VertexInTriangleIndex = 0; VertexInTriangleIndex < 3; ++VertexInTriangleIndex)
			{
				BoundsMin.X = BoundsMin.X > Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.X ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.X : BoundsMin.X;
				BoundsMin.Y = BoundsMin.Y > Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Y ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Y : BoundsMin.Y;
				BoundsMin.Z = BoundsMin.Z > Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Z ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Z : BoundsMin.Z;

				BoundsMax.X = BoundsMax.X < Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.X ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.X : BoundsMax.X;
				BoundsMax.Y = BoundsMax.Y < Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Y ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Y : BoundsMax.Y;
				BoundsMax.Z = BoundsMax.Z < Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Z ? Triangles[TriangleIndex].Vertices[VertexInTriangleIndex].Position.Z : BoundsMax.Z;
			}
		}

		FVector Origin = ((BoundsMax - BoundsMin) * .5f) + BoundsMin;
		FVector Delta = BoundsMax - BoundsMin;
		return FBoxSphereBounds(Origin, Delta, Delta.Size()).TransformBy(LocalToWorld);
	}
	else
	{
		return FBoxSphereBounds();
	}
}

bool UHMMeshComponent::GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData)
{
	FTriIndices Triangle;
	for (int32 i = 0; i < Triangles.Num(); i++)
	{
		const FHMMeshTriangle& CurrentTriangle = Triangles[i];

		Triangle.v0 = CollisionData->Vertices.Add(CurrentTriangle.Vertices[0].Position);
		Triangle.v1 = CollisionData->Vertices.Add(CurrentTriangle.Vertices[1].Position);
		Triangle.v2 = CollisionData->Vertices.Add(CurrentTriangle.Vertices[2].Position);
		CollisionData->Indices.Add(Triangle);
		CollisionData->MaterialIndices.Add(i);
	}
	CollisionData->bFlipNormals = true;
	return true;
}

bool UHMMeshComponent::ContainsPhysicsTriMeshData(bool InUseAllTriData) const
{
	return (Triangles.Num() > 0);
}

void UHMMeshComponent::UpdateBodySetup()
{
	if (ModelBodySetup == NULL)
	{
		ModelBodySetup = NewObject<UBodySetup>(this);
		ModelBodySetup->CollisionTraceFlag = CTF_UseComplexAsSimple;
		ModelBodySetup->bMeshCollideAll = true;
	}
}

void UHMMeshComponent::UpdateCollision()
{
	if (bPhysicsStateCreated)
	{
		DestroyPhysicsState();
		UpdateBodySetup();
		CreatePhysicsState();

		ModelBodySetup->InvalidatePhysicsData();
		ModelBodySetup->CreatePhysicsMeshes();
	}
}

UBodySetup* UHMMeshComponent::GetBodySetup()
{
	UpdateBodySetup();
	return ModelBodySetup;
}


