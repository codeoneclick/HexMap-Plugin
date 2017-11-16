
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMTile.h"
#include "HMTileNavigationComponent.h"
#include "HMNavigation.h"

FHMNavigationAbstractNode::~FHMNavigationAbstractNode()
{
	Parent.Reset();
	RemoveChildren();
}

void FHMNavigationAbstractNode::SetParent(const TSharedPtr<FHMNavigationAbstractNode>& Parent_)
{
	Parent = Parent_;
}

TSharedPtr<FHMNavigationAbstractNode> FHMNavigationAbstractNode::GetParent() const
{
	return Parent.Pin();
}

void FHMNavigationAbstractNode::AddChild(const TSharedPtr<FHMNavigationAbstractNode>& Child, float Distance)
{
	FHMNavigationChildNode ChildNode;
	ChildNode.Node = Child;
	ChildNode.Distance = Distance;
	Children.Add(ChildNode);
}

TArray<FHMNavigationChildNode>& FHMNavigationAbstractNode::GetChildren()
{
	return Children;
}

void FHMNavigationAbstractNode::RemoveChildren()
{
	Children.Empty();
}

void FHMNavigationAStarNode::Init(float F_, float G_, float H_, bool bIsOpen_, bool bIsClosed_)
{
	F = F_;
	G = G_;
	H = H_;

	bIsOpen = bIsOpen_;
	bIsClosed = bIsClosed_;
}

void FHMNavigationAStarNode::Release()
{
	bIsOpen = bIsClosed = false;
	F = G = H = 0.f;
	Parent = nullptr;
}

float FHMNavigationAStarNode::DistanceTo(const TSharedPtr<FHMNavigationAStarNode>& Node) const
{
	ensure(false);
	return 0.f;
}

TArray<TSharedPtr<FHMNavigationAStarNode>> FHMNavigationAStar::ConstructPath(const TSharedPtr<FHMNavigationAStarNode>& Node)
{
	TArray<TSharedPtr<FHMNavigationAStarNode>> Path;
	Path.Add(Node);
	TSharedPtr<FHMNavigationAStarNode> Parent = StaticCastSharedPtr<FHMNavigationAStarNode>(Node->GetParent());
	while (Parent.Get())
	{
		Path.Add(Parent);
		Parent = StaticCastSharedPtr<FHMNavigationAStarNode>(Parent->GetParent());
	}
	return Path;
}

TArray<TSharedPtr<FHMNavigationAStarNode>> FHMNavigationAStar::GetPath(const TSharedPtr<FHMNavigationAStarNode>& StartNode, const TSharedPtr<FHMNavigationAStarNode>& GoalNode)
{
	TSharedPtr<FHMNavigationAStarNode> CurrentNode = nullptr;
	TSharedPtr<FHMNavigationAStarNode> ChildNode = nullptr;
	float F, G, H;

	std::make_heap(OpenNodes.begin(), OpenNodes.end(), FHMNavigationAStarNodeComparator());
	PushToOpen(StartNode);

	while (!OpenNodes.empty())
	{
		std::sort(OpenNodes.begin(), OpenNodes.end(), FHMNavigationAStarNodeComparator());

		CurrentNode = OpenNodes[0];
		PopFromOpen(CurrentNode);

		CurrentNode->SetClosed(true);
		ClosedNodes.push_back(CurrentNode);

		if (CurrentNode == GoalNode)
		{
			return ConstructPath(CurrentNode);
		}

		for (FHMNavigationChildNode& ChildNodeIt : CurrentNode->GetChildren())
		{
			ChildNode = StaticCastSharedPtr<FHMNavigationAStarNode>(ChildNodeIt.Node);
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
	TArray<TSharedPtr<FHMNavigationAStarNode>> Empty;
	return Empty;
}

void FHMNavigationAStar::PushToOpen(const TSharedPtr<FHMNavigationAStarNode>& Node)
{
	OpenNodes.push_back(Node);
	std::push_heap(OpenNodes.begin(), OpenNodes.end(), FHMNavigationAStarNodeComparator());
	Node->SetOpen(true);
}

void FHMNavigationAStar::PopFromOpen(const TSharedPtr<FHMNavigationAStarNode>& Node)
{
	std::pop_heap(OpenNodes.begin(), OpenNodes.end(), FHMNavigationAStarNodeComparator());
	OpenNodes.pop_back();
	Node->SetOpen(false);
}

void FHMNavigationAStar::ReleaseNodes()
{
	for (const TSharedPtr<FHMNavigationAStarNode>& Node : OpenNodes)
	{
		Node->Release();
	}
	for (const TSharedPtr<FHMNavigationAStarNode>& Node : ClosedNodes)
	{
		Node->Release();
	}
}

void FHMNavigationAStar::Clear()
{
	ReleaseNodes();
	OpenNodes.clear();
	ClosedNodes.clear();
}

void FHMNavigationConcreteNode::SetPassable(bool bValue)
{
	bIsPassable = bValue;
}

bool FHMNavigationConcreteNode::GetPassable() const
{
	return bIsPassable;
}

float FHMNavigationConcreteNode::DistanceToLocal(const TSharedPtr<FHMNavigationAStarNode>& Node) const
{
	return 1.f;
}

float FHMNavigationConcreteNode::DistanceTo(const TSharedPtr<FHMNavigationAStarNode>& Node) const
{
	return FVector2D::Distance(FVector2D(X, Y), FVector2D(Node->GetX(), Node->GetY()));
}

void FHMNavigationConcreteNode::SetHeight(float Height_)
{
	Height = Height_;
}

float FHMNavigationConcreteNode::GetHeight() const
{
	return Height;
}

void FHMNavigation::AddTile(class AHMTile* Tile)
{
	Tiles.Add(Tile);
}

void FHMNavigation::RemoveTile(class AHMTile* Tile)
{
	Tiles.Remove(Tile);
}

void FHMNavigation::ConstructNavigation(bool bReConstruct)
{
	if (bReConstruct)
	{
		for (auto NavigationNodeIt : Nodes)
		{
			TSharedPtr<FHMNavigationConcreteNode> NavigationNode = NavigationNodeIt.Value;
			NavigationNode->RemoveChildren();
		}
		Nodes.Empty();

		for (AHMTile* Tile : Tiles)
		{
			FVector Location = Tile->GetActorLocation();
			TSharedPtr<FHMNavigationConcreteNode> NavigationNode = MakeShareable(new FHMNavigationConcreteNode());
			bool bIsPassable = false;
			UHMTileNavigationComponent* TileNavigationComponent = Tile->FindComponentByClass<UHMTileNavigationComponent>();
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
			Nodes.Add(Tile, NavigationNode);
		}
	}

	for (auto NavigationNodeIt : Nodes)
	{
		AHMTile* Tile = NavigationNodeIt.Key;
		TSharedPtr<FHMNavigationConcreteNode> NavigationNode = NavigationNodeIt.Value;
		NavigationNode->RemoveChildren();
		float TileLocationZ = Tile->GetActorLocation().Z;
		for (AHMTile* NeighbourTile: Tile->Neighbours)
		{
			float NeighbourTileLocationZ = NeighbourTile->GetActorLocation().Z;
			auto NeighbourNavigationNodeIt = Nodes.Find(NeighbourTile);
			if (NeighbourNavigationNodeIt)
			{
				TSharedPtr<FHMNavigationConcreteNode> NeighbourNavigationNode = *NeighbourNavigationNodeIt;
				if (NeighbourNavigationNode->GetPassable() && FMath::Abs(TileLocationZ - NeighbourTileLocationZ) <= MaxPassableDifferential)
				{
					NavigationNode->AddChild(NeighbourNavigationNode, NavigationNode->DistanceToLocal(NeighbourNavigationNode));
				}
			}
		}
	}
}

void FHMNavigation::SetTileHeight(float TileHeight_)
{
	TileHeight = TileHeight_;
}

void FHMNavigation::SetMaxPassableDifferential(float MaxPassableDifferential_)
{
	MaxPassableDifferential = MaxPassableDifferential_;
}

bool FHMNavigation::GetPath(AHMTile* StartTile, AHMTile* GoalTile, TArray<FVector>& OutSolution)
{
	bool bResult = false;
	if (StartTile && GoalTile)
	{
		auto StartNavigationNodeIt = Nodes.Find(StartTile);
		auto GoalNavigationNodeIt = Nodes.Find(GoalTile);
		if (StartNavigationNodeIt && GoalNavigationNodeIt)
		{
			TSharedPtr<FHMNavigationConcreteNode> StartNavigationNode = *StartNavigationNodeIt;
			TSharedPtr<FHMNavigationConcreteNode> GoalNavigationNode = *GoalNavigationNodeIt;

			TSharedPtr<FHMNavigationAStar> Algorithm = MakeShareable(new FHMNavigationAStar());
			TArray<TSharedPtr<FHMNavigationAStarNode>> Solution = Algorithm->GetPath(StartNavigationNode, GoalNavigationNode);
			Algorithm->Clear();
			if (Solution.Num() > 0)
			{
				for (const TSharedPtr<FHMNavigationAStarNode>& AStarNode : Solution)
				{
					TSharedPtr<FHMNavigationConcreteNode> ConcreteNode = StaticCastSharedPtr<FHMNavigationConcreteNode>(AStarNode);
					OutSolution.Add(FVector(AStarNode->GetX(), AStarNode->GetY(), ConcreteNode->GetHeight() + TileHeight));
				}
				bResult = true;
			}
		}
	}
	return bResult;
}


