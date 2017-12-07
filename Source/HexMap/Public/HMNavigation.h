// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <limits>
#include <vector>
#include "HMNavigation.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct HEXMAP_API FHMNavigationChildNode
{
	GENERATED_USTRUCT_BODY()

public:

	TSharedPtr<class FHMNavigationAbstractNode> Node = nullptr;
	float Distance = -1.f;
};

class HEXMAP_API FHMNavigationAbstractNode
{
protected:

	TWeakPtr<FHMNavigationAbstractNode> Parent;
	TArray<FHMNavigationChildNode> Children;

public:

	FHMNavigationAbstractNode() = default;
	virtual ~FHMNavigationAbstractNode();

	void SetParent(const TSharedPtr<FHMNavigationAbstractNode>& Parent_);
	TSharedPtr<FHMNavigationAbstractNode> GetParent() const;

	void AddChild(const TSharedPtr<FHMNavigationAbstractNode>& Child, float Distance);
	TArray<FHMNavigationChildNode>& GetChildren();
	void RemoveChildren();
};

class HEXMAP_API FHMNavigationAStarNode : public FHMNavigationAbstractNode
{
protected:

	float F = 0.f;
	float G = 0.f;
	float H = 0.f;
	float X = 0.f;
	float Y = 0.f;
	bool bIsOpen = false;
	bool bIsClosed = false;

public:

	FHMNavigationAStarNode() = default;
	virtual ~FHMNavigationAStarNode() = default;

	void Init(float F_, float G_, float H_, bool bIsOpen_, bool bIsClosed_);
	void Release();

	inline void SetPosition(float X_, float Y_)
	{
		X = X_;
		Y = Y_;
	};

	inline void SetF(float F_)
	{
		F = F_;
	};

	inline void SetG(float G_)
	{
		G = G_;
	};

	inline void SetH(float H_)
	{
		H = H_;
	};

	inline void SetOpen(bool bValue)
	{
		bIsOpen = bValue;
	};

	inline void SetClosed(bool bValue)
	{
		bIsClosed = bValue;
	};

	inline float GetX() const
	{
		return X;
	};

	inline float GetY() const
	{
		return Y;
	};

	inline float GetF() const
	{
		return F;
	};

	inline float GetG() const
	{
		return G;
	};

	inline float GetH() const
	{
		return H;
	};

	inline bool IsOpen() const
	{
		return bIsOpen;
	};

	inline bool IsClosed() const
	{
		return bIsClosed;
	};

	virtual float DistanceTo(const TSharedPtr<FHMNavigationAStarNode>& Node) const;
};

class HEXMAP_API FHMNavigationConcreteNode : public FHMNavigationAStarNode
{
protected:

	bool bIsPassable = true;
	float Height = 0.f;

public:

	FHMNavigationConcreteNode() = default;
	virtual ~FHMNavigationConcreteNode() = default;

	void SetPassable(bool bValue);
	bool GetPassable() const;

	float DistanceToLocal(const TSharedPtr<FHMNavigationAStarNode>& Node) const;
	float DistanceTo(const TSharedPtr<FHMNavigationAStarNode>& Node) const override;

	void SetHeight(float Height_);
	float GetHeight() const;
};

struct FHMNavigationAStarNodeComparator
{
	bool operator() (const TSharedPtr<FHMNavigationAStarNode>& Node1, const TSharedPtr<FHMNavigationAStarNode>& Node2) const
	{
		return Node1->GetF() < Node2->GetF();
	}
};

class HEXMAP_API FHMNavigationAStar
{
private:

	void ReleaseNodes();
	void PushToOpen(const TSharedPtr<FHMNavigationAStarNode>& Node);
	void PopFromOpen(const TSharedPtr<FHMNavigationAStarNode>& Node);

	std::vector<TSharedPtr<FHMNavigationAStarNode>> OpenNodes;
	std::vector<TSharedPtr<FHMNavigationAStarNode>> ClosedNodes;

	inline float DistanceBetween(const TSharedPtr<FHMNavigationAStarNode>& Node1, const TSharedPtr<FHMNavigationAStarNode>& Node2) const
	{
		return Node1->DistanceTo(Node2);
	};

	TArray<TSharedPtr<FHMNavigationAStarNode>> ConstructPath(const TSharedPtr<FHMNavigationAStarNode>& Node);

public:

	TArray<TSharedPtr<FHMNavigationAStarNode>> GetPath(const TSharedPtr<FHMNavigationAStarNode>& StartNode, const TSharedPtr<FHMNavigationAStarNode>& GoalNode);
	void Clear();
};

class HEXMAP_API FHMNavigation
{
protected:

	TArray<class AHMTile*> Tiles;
	TMap<class AHMTile*, TSharedPtr<FHMNavigationConcreteNode>> Nodes;

	float MaxPassableDifferential = std::numeric_limits<float>::max();

public:

	virtual ~FHMNavigation() = default;

	void AddTile(class AHMTile* Tile);
	void RemoveTile(class AHMTile* Tile);
	virtual void ConstructNavigation(bool bReConstruct = false);
	void SetMaxPassableDifferential(float MaxPassableDifferential_);

	bool GetPath(AHMTile* StartTile, AHMTile* GoalTile, TArray<FVector>& OutSolution);
};
