// Fill out your copyright notice in the Description page of Project Settings.
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexUtils.h"
#include "HexMapGrid.h"
#include "HexNavigation.h"
#include "HexMapTile.h"
#include "HexMapTileLocationComponent.h"
#include "HexMapTileNavigationComponent.h"

void FHexNavigationAbstractNode::SetParent(const TSharedPtr<FHexNavigationAbstractNode>& Parent_)
{
	Parent = Parent_;
}

TSharedPtr<FHexNavigationAbstractNode> FHexNavigationAbstractNode::GetParent() const
{
	return Parent.Pin();
}

void FHexNavigationAbstractNode::AddChild(const TSharedPtr<FHexNavigationAbstractNode>& Child, float Distance)
{
	FHexNavigationChildNode ChildNode;
	ChildNode.Node = Child;
	ChildNode.Distance = Distance;
	Children.Add(ChildNode);
}

TArray<FHexNavigationChildNode>& FHexNavigationAbstractNode::GetChildren()
{
	return Children;
}

void FHexNavigationAbstractNode::RemoveChildren()
{
	Children.Empty();
}

void FHexNavigationAStarNode::Init(float F_, float G_, float H_, bool bIsOpen_, bool bIsClosed_)
{
	F = F_;
	G = G_;
	H = H_;

	bIsOpen = bIsOpen_;
	bIsClosed = bIsClosed_;
}

void FHexNavigationAStarNode::Release()
{
	bIsOpen = bIsClosed = false;
	F = G = H = 0.f;
	Parent = nullptr;
}

float FHexNavigationAStarNode::DistanceTo(const TSharedPtr<FHexNavigationAStarNode>& Node) const
{
	ensure(false);
	return 0.f;
}

TArray<TSharedPtr<FHexNavigationAStarNode>> FHexNavigationAStar::ConstructPath(const TSharedPtr<FHexNavigationAStarNode>& Node)
{
	TArray<TSharedPtr<FHexNavigationAStarNode>> Path;
	Path.Add(Node);
	TSharedPtr<FHexNavigationAStarNode> Parent = StaticCastSharedPtr<FHexNavigationAStarNode>(Node->GetParent());
	while (Parent.Get())
	{
		Path.Add(Parent);
		Parent = StaticCastSharedPtr<FHexNavigationAStarNode>(Parent->GetParent());
	}
	return Path;
}

TArray<TSharedPtr<FHexNavigationAStarNode>> FHexNavigationAStar::GetPath(const TSharedPtr<FHexNavigationAStarNode>& StartNode, const TSharedPtr<FHexNavigationAStarNode>& GoalNode)
{
	TSharedPtr<FHexNavigationAStarNode> CurrentNode = nullptr;
	TSharedPtr<FHexNavigationAStarNode> ChildNode = nullptr;
	float F, G, H;

	std::make_heap(OpenNodes.begin(), OpenNodes.end(), FHexNavigationAStarNodeComparator());
	PushToOpen(StartNode);

	while (!OpenNodes.empty())
	{
		std::sort(OpenNodes.begin(), OpenNodes.end(), FHexNavigationAStarNodeComparator());

		CurrentNode = OpenNodes[0];
		PopFromOpen(CurrentNode);

		CurrentNode->SetClosed(true);
		ClosedNodes.push_back(CurrentNode);

		if (CurrentNode == GoalNode)
		{
			return ConstructPath(CurrentNode);
		}

		for (FHexNavigationChildNode& ChildNodeIt : CurrentNode->GetChildren())
		{
			ChildNode = StaticCastSharedPtr<FHexNavigationAStarNode>(ChildNodeIt.Node);
			G = CurrentNode->GetG() + ChildNodeIt.Distance;
			if ((ChildNode->IsOpen() || ChildNode->IsClosed()) && ChildNode->GetG() < G)
			{
				continue;
			}

			H = DistanceBetween(ChildNode, GoalNode);
			F = G + H;
			ChildNode->SetF(F);
			ChildNode->SetG(G);
			ChildNode->SetH(H);
			ChildNode->SetParent(CurrentNode);

			if (ChildNode->IsClosed())
			{
				ChildNode->SetClosed(false);
			}

			if (!ChildNode->IsOpen())
			{
				PushToOpen(ChildNode);
			}
		}
	}
	TArray<TSharedPtr<FHexNavigationAStarNode>> Empty;
	return Empty;
}

void FHexNavigationAStar::PushToOpen(const TSharedPtr<FHexNavigationAStarNode>& Node)
{
	OpenNodes.push_back(Node);
	std::push_heap(OpenNodes.begin(), OpenNodes.end(), FHexNavigationAStarNodeComparator());
	Node->SetOpen(true);
}

void FHexNavigationAStar::PopFromOpen(const TSharedPtr<FHexNavigationAStarNode>& Node)
{
	std::pop_heap(OpenNodes.begin(), OpenNodes.end(), FHexNavigationAStarNodeComparator());
	OpenNodes.pop_back();
	Node->SetOpen(false);
}

void FHexNavigationAStar::ReleaseNodes()
{
	for (const TSharedPtr<FHexNavigationAStarNode>& Node : OpenNodes)
	{
		Node->Release();
	}
	for (const TSharedPtr<FHexNavigationAStarNode>& Node : ClosedNodes)
	{
		Node->Release();
	}
}

void FHexNavigationAStar::Clear()
{
	ReleaseNodes();
	OpenNodes.clear();
	ClosedNodes.clear();
}

void FHexNavigationConcreteNode::SetPassable(bool bValue)
{
	bIsPassable = bValue;
}

bool FHexNavigationConcreteNode::GetPassable() const
{
	return bIsPassable;
}

float FHexNavigationConcreteNode::DistanceToLocal(const TSharedPtr<FHexNavigationAStarNode>& Node) const
{
	return 1.f;
}

float FHexNavigationConcreteNode::DistanceTo(const TSharedPtr<FHexNavigationAStarNode>& Node) const
{
	return FVector2D::Distance(FVector2D(X, Y), FVector2D(Node->GetX(), Node->GetY()));
}

void FHexNavigationConcreteNode::SetHeight(float Height_)
{
	Height = Height_;
}

float FHexNavigationConcreteNode::GetHeight() const
{
	return Height;
}

void FHexNavigation::RegisterHexTileLocationComponent(class UHexMapTileLocationComponent* TileLocationComponent)
{
	RegisteredTiles.Add(TileLocationComponent);
}

void FHexNavigation::UnRegisterHexTileLocationComponent(class UHexMapTileLocationComponent* TileLocationComponent)
{
	RegisteredTiles.Remove(TileLocationComponent);
}

void FHexNavigation::UpdateNavigationNodes(bool bReConstruct)
{
	if (bReConstruct)
	{
		NavigationNodes.Empty();

		for (UHexMapTileLocationComponent* TileLocationComponent : RegisteredTiles)
		{
			if (TileLocationComponent->LinkedTile)
			{
				FVector Location = TileLocationComponent->LinkedTile->GetActorLocation();
				TSharedPtr<FHexNavigationConcreteNode> NavigationNode = MakeShareable(new FHexNavigationConcreteNode());
				bool bIsPassable = false;
				UHexMapTileNavigationComponent* TileNavigationComponent = TileLocationComponent->LinkedTile->FindComponentByClass<UHexMapTileNavigationComponent>();
				if (TileNavigationComponent)
				{
					bIsPassable = TileNavigationComponent->bIsPassable;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Can't find TileNavigationComponent!"));
				}

				NavigationNode->SetPassable(bIsPassable);
				NavigationNode->SetPosition(Location.X, Location.Y);
				NavigationNode->SetHeight(Location.Z);
				NavigationNodes.Add(TileLocationComponent, NavigationNode);
			}
		}
	}

	for (auto NavigationNodeIt : NavigationNodes)
	{
		UHexMapTileLocationComponent* TileLocationComponent = NavigationNodeIt.Key;
		TSharedPtr<FHexNavigationConcreteNode> NavigationNode = NavigationNodeIt.Value;
		NavigationNode->RemoveChildren();
		float TileLocationComponentHeight = TileLocationComponent->LinkedTile->GetActorLocation().Z;
		for (UHexMapTileLocationComponent* NeighbourTileLocationComponent : TileLocationComponent->Neighbours)
		{
			float NeighbourTileLocationComponentHeight = NeighbourTileLocationComponent->LinkedTile->GetActorLocation().Z;
			auto NeighbourNavigationNodeIt = NavigationNodes.Find(NeighbourTileLocationComponent);
			if (NeighbourNavigationNodeIt)
			{
				TSharedPtr<FHexNavigationConcreteNode> NeighbourNavigationNode = *NeighbourNavigationNodeIt;
				if (NeighbourNavigationNode->GetPassable() && FMath::Abs(TileLocationComponentHeight - NeighbourTileLocationComponentHeight) < MaxPassableHeight)
				{
					NavigationNode->AddChild(NeighbourNavigationNode, NavigationNode->DistanceToLocal(NeighbourNavigationNode));
				}
			}
		}
	}
}

void FHexNavigation::SetTileHeight(float TileHeight_)
{
	TileHeight = TileHeight_;
}

void FHexNavigation::SetMaxPassableHeight(float MaxPassableHeight_)
{
	MaxPassableHeight = MaxPassableHeight_;
}

bool FHexNavigation::GetPath(UHexMapTileLocationComponent* StartTileLocationComponent,
							 UHexMapTileLocationComponent* GoalTileLocationComponent,
							 TArray<FVector>& OutSolution)
{
	bool bResult = false;
	if (StartTileLocationComponent && GoalTileLocationComponent)
	{
		auto StartNavigationNodeIt = NavigationNodes.Find(StartTileLocationComponent);
		auto GoalNavigationNodeIt = NavigationNodes.Find(GoalTileLocationComponent);
		if (StartNavigationNodeIt && GoalNavigationNodeIt)
		{
			TSharedPtr<FHexNavigationConcreteNode> StartNavigationNode = *StartNavigationNodeIt;
			TSharedPtr<FHexNavigationConcreteNode> GoalNavigationNode = *GoalNavigationNodeIt;

			TSharedPtr<FHexNavigationAStar> Algorithm = MakeShareable(new FHexNavigationAStar());
			TArray<TSharedPtr<FHexNavigationAStarNode>> Solution = Algorithm->GetPath(StartNavigationNode, GoalNavigationNode);
			Algorithm->Clear();
			if (Solution.Num() > 0)
			{
				for (const TSharedPtr<FHexNavigationAStarNode>& AStarNode : Solution)
				{
					TSharedPtr<FHexNavigationConcreteNode> ConcreteNode = StaticCastSharedPtr<FHexNavigationConcreteNode>(AStarNode);
					OutSolution.Add(FVector(AStarNode->GetX(), AStarNode->GetY(), ConcreteNode->GetHeight() + TileHeight));
				}
				bResult = true;
			}
		}
	}
	return bResult;
}
