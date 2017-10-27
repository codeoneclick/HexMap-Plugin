// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct HEXMAP_API FHexNavigationChildNode
{
public:
	TSharedPtr<class FHexNavigationAbstractNode> Node = nullptr;
	float Distance = -1.f;
};

class HEXMAP_API FHexNavigationAbstractNode
{
protected:

	TWeakPtr<FHexNavigationAbstractNode> Parent;
	TArray<FHexNavigationChildNode> Children;

public:

	FHexNavigationAbstractNode() = default;
	virtual ~FHexNavigationAbstractNode() = default;

	void SetParent(const TSharedPtr<FHexNavigationAbstractNode>& Parent_);
	TSharedPtr<FHexNavigationAbstractNode> GetParent() const;

	void AddChild(const TSharedPtr<FHexNavigationAbstractNode>& Child, float Distance);
	TArray<FHexNavigationChildNode>& GetChildren();
	void RemoveChildren();
};

class HEXMAP_API FHexNavigationAStarNode : public FHexNavigationAbstractNode
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

	FHexNavigationAStarNode() = default;
	virtual ~FHexNavigationAStarNode() = default;

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

	virtual float DistanceTo(const TSharedPtr<FHexNavigationAStarNode>& Node) const;
};

class HEXMAP_API FHexNavigationConcreteNode : public FHexNavigationAStarNode
{
protected:

	bool bIsPassable = true;
	float Height = 0.f;

public:

	FHexNavigationConcreteNode() = default;
	virtual ~FHexNavigationConcreteNode() = default;

	void SetPassable(bool bValue);
	bool GetPassable() const;

	float DistanceToLocal(const TSharedPtr<FHexNavigationAStarNode>& Node) const;
	float DistanceTo(const TSharedPtr<FHexNavigationAStarNode>& Node) const override;

	void SetHeight(float Height_);
	float GetHeight() const;
};

struct FHexNavigationAStarNodeComparator
{
	bool operator() (const TSharedPtr<FHexNavigationAStarNode>& Node1, const TSharedPtr<FHexNavigationAStarNode>& Node2) const
	{
		return Node1->GetF() < Node2->GetF();
	}
};

class HEXMAP_API FHexNavigationAStar
{
private:

	void ReleaseNodes();
	void PushToOpen(const TSharedPtr<FHexNavigationAStarNode>& Node);
	void PopFromOpen(const TSharedPtr<FHexNavigationAStarNode>& Node);

	std::vector<TSharedPtr<FHexNavigationAStarNode>> OpenNodes;
	std::vector<TSharedPtr<FHexNavigationAStarNode>> ClosedNodes;

	inline float DistanceBetween(const TSharedPtr<FHexNavigationAStarNode>& Node1, const TSharedPtr<FHexNavigationAStarNode>& Node2) const
	{
		return Node1->DistanceTo(Node2);
	};

	TArray<TSharedPtr<FHexNavigationAStarNode>> ConstructPath(const TSharedPtr<FHexNavigationAStarNode>& Node);

public:

	TArray<TSharedPtr<FHexNavigationAStarNode>> GetPath(const TSharedPtr<FHexNavigationAStarNode>& StartNode, const TSharedPtr<FHexNavigationAStarNode>& GoalNode);
	void Clear();
};

class HEXMAP_API FHexNavigation 
{
protected:

	TArray<class UHexMapTileLocationComponent*> RegisteredTiles;
	TMap<class UHexMapTileLocationComponent*, TSharedPtr<FHexNavigationConcreteNode>> NavigationNodes;

	float TileHeight = 0.f;

public:

	virtual ~FHexNavigation() = default;

	void RegisterHexTileLocationComponent(class UHexMapTileLocationComponent* TileLocationComponent);
	void UnRegisterHexTileLocationComponent(class UHexMapTileLocationComponent* TileLocationComponent);
	virtual void UpdateNavigationNodes(bool bReConstruct = false);
	void SetTileHeight(float TileHeight_);

	bool GetPath(UHexMapTileLocationComponent* StartTileLocationComponent,
				 UHexMapTileLocationComponent* GoalTileLocationComponent,
				 TArray<FVector>& OutSolution);
};

