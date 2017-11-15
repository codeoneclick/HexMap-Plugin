
#include "HexMapPrivatePCH.h"
#include "HMActorNavigationComponent.h"
#include "HMGrid.h"
#include "HMTile.h"
#include "HMUtilities.h"
#include "HMTileNavigationComponent.h"
#include "HMGridNavigationComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "HMActorPlacementComponent.h"

UHMActorNavigationComponent::UHMActorNavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/HexMap/M_HMNavigationDebug.M_HMNavigationDebug'"));
	DebugNavigationMaterial = Material.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/HexMap/SM_Plane.SM_Plane'"));
	DebugNavigationMesh = StaticMesh.Object;
}

void UHMActorNavigationComponent::BeginPlay()
{
	Super::BeginPlay();
	LastGoalLocation = GetComponentLocation();

	if (bDebug)
	{
		USplineComponent* SplineComponent = NewObject<USplineComponent>(this);
		SplineComponent->SetupAttachment(this);
		SplineComponent->RegisterComponent();

		SplineComponent->bAbsoluteLocation = true;
		SplineComponent->bAbsoluteRotation = true;
	}
}

void UHMActorNavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsHasSolution && !IsGoalLocationReached(LastGoalLocation))
	{
		if (Solution.Num() > 1)
		{
			bool bIsNextNavigationPointReached = IsGoalLocationReached(Solution.Last(), AcceptableRadiusBetweenNavigationPoints);
			if (bIsNextNavigationPointReached)
			{
				Solution.Pop();
			}
		}
		if (GetPath(LastGoalLocation) && PawnController)
		{
			UNavigationSystem::SimpleMoveToLocation(PawnController, GetNextNavigationPoint());
		}
		if (bDebug)
		{
			UpdateSpline(true);
		}
	}
	else
	{
		if (PawnController)
		{
			UPathFollowingComponent* PathFollowingComponent = PawnController->FindComponentByClass<UPathFollowingComponent>();
			PathFollowingComponent->OnPathFinished(EPathFollowingResult::Success, FPathFollowingResultFlags::None);
		}
		if (bDebug)
		{
			UpdateSpline(false);
		}
	}
	AcceptableRadius = FMath::Max(AcceptableRadiusToLastNavigationPoint, AcceptableRadius);
}

bool UHMActorNavigationComponent::GetPath(const FVector& GoalLocation)
{
	if (!GridNavigationComponent)
	{
		// TODO:
		return false;
	}
	bool bIsUseSamePath = false;
	FVector GoalLocation2D = GoalLocation;
	GoalLocation2D.Z = 0.f;
	FVector LastGoalLocation2D = LastGoalLocation;
	LastGoalLocation2D.Z = 0.f;
	if (FVector::Distance(GoalLocation2D, LastGoalLocation2D) <= 1.f && Solution.Num() > 0)
	{
		bIsUseSamePath = true;
		for (FVector SolutionPoint : Solution)
		{
			AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
			FHMCoord HexCoord = FHMUtilities::ToHex(GetWorld(), SolutionPoint);
			AHMTile** Tile = Grid->TilesToLocationsLinkages.Find(HexCoord.ToVec());
			ensure(Tile != nullptr);
			if (Tile)
			{
				UHMTileNavigationComponent* TileNavigationComponent = (*Tile)->FindComponentByClass<UHMTileNavigationComponent>();
				ensure(TileNavigationComponent != nullptr);
				if (!TileNavigationComponent->IsPassable(this))
				{
					bIsUseSamePath = false;
					break;
				}
				else
				{
					// TODO:
				}
			}
			else
			{
				// TODO:
			}
		}
	}

	if (!bIsUseSamePath)
	{
		Solution.Empty();
		FVector CurrentLocation = GetComponentLocation();
		bIsHasSolution = GridNavigationComponent->GetPath(CurrentLocation, GoalLocation, Solution, SpreadingRadius);
		if (Solution.Num() > 1)
		{
			Solution.Pop();
		}
	}
	bIsHasSolution = Solution.Num() > 0;
	if (bIsHasSolution && !bIsUseSamePath)
	{
		LastGoalLocation = GoalLocation;
		if (FHMCoord::Compare(FHMUtilities::ToNearestHex(GetWorld(), GoalLocation), FHMUtilities::ToNearestHex(GetWorld(), Solution[0])))
		{
			Solution[0].X = GoalLocation.X;
			Solution[0].Y = GoalLocation.Y;
		}
	}
	return bIsHasSolution;
}

FVector UHMActorNavigationComponent::GetNextNavigationPoint()
{
	return Solution.Last();
}

bool UHMActorNavigationComponent::IsGoalLocationReached(const FVector& GoalLocation, float AcceptableRadius_) const
{
	FVector CurrentLocation2D = GetComponentLocation();
	CurrentLocation2D.Z = 0.f;
	FVector GoalLocation2D = GoalLocation;
	GoalLocation2D.Z = 0.f;
	const float Distance = FVector::Distance(CurrentLocation2D, GoalLocation2D);
	const float ComparisonDistance = AcceptableRadius_ >= 0.f ? AcceptableRadius_ : AcceptableRadius;
	return Distance < ComparisonDistance;
}

bool UHMActorNavigationComponent::IsGoalActorReached(AActor* Actor, float AcceptableRadius_) const
{
	FVector GoalLocation = Actor->GetActorLocation();
	UHMActorPlacementComponent* PlacementComponent = Actor->FindComponentByClass<UHMActorPlacementComponent>();
	if (PlacementComponent)
	{
		FVector CurrentLocation = GetComponentLocation();
		int32 InitialSolutionCount = std::numeric_limits<int32>::max();
		TArray<FVector> PlacementsLocations = PlacementComponent->PlacementsLocations;
		for (FVector PlacementLocation : PlacementsLocations)
		{
			TArray<FVector> InitialSolution;
			bool bIsHasInitialSolution = GridNavigationComponent->GetPath(CurrentLocation, PlacementLocation, InitialSolution, SpreadingRadius);
			if (bIsHasInitialSolution && InitialSolution.Num() < InitialSolutionCount)
			{
				InitialSolutionCount = InitialSolution.Num();
				GoalLocation = PlacementLocation;
			}
		}
	}
	FVector CurrentLocation2D = GetComponentLocation();
	CurrentLocation2D.Z = 0.f;
	FVector GoalLocation2D = GoalLocation;
	GoalLocation2D.Z = 0.f;
	const float Distance = FVector::Distance(CurrentLocation2D, GoalLocation2D);
	const float ComparisonDistance = AcceptableRadius_ >= 0.f ? AcceptableRadius_ : AcceptableRadius;
	return Distance < ComparisonDistance;
}

void UHMActorNavigationComponent::Interrupt()
{
	Solution.Empty();
	LastGoalLocation = GetComponentLocation();
}

bool UHMActorNavigationComponent::MoveToLocation(AController* Controller, const FVector& GoalLocation)
{
	bool bResult = GetPath(GoalLocation);
	PawnController = Controller;
	return bResult;
}

bool UHMActorNavigationComponent::MoveToActor(AController* Controller, AActor* Actor)
{
	bool bResult = false;
	if (Actor)
	{
		FVector GoalLocation = Actor->GetActorLocation();
		UHMActorPlacementComponent* PlacementComponent = Actor->FindComponentByClass<UHMActorPlacementComponent>();
		if (PlacementComponent)
		{
			FVector CurrentLocation = GetComponentLocation();
			int32 InitialSolutionCount = std::numeric_limits<int32>::max();
			TArray<FVector> PlacementsLocations = PlacementComponent->PlacementsLocations;
			for (FVector PlacementLocation : PlacementsLocations)
			{
				TArray<FVector> InitialSolution;
				bool bIsHasInitialSolution = GridNavigationComponent->GetPath(CurrentLocation, PlacementLocation, InitialSolution, SpreadingRadius);
				if (bIsHasInitialSolution && InitialSolution.Num() < InitialSolutionCount)
				{
					InitialSolutionCount = InitialSolution.Num();
					GoalLocation = PlacementLocation;
				}
			}
		}
		bResult = GetPath(GoalLocation);
		PawnController = Controller;
	}
	return bResult;
}

void UHMActorNavigationComponent::UpdateSpline(bool bVisible)
{
	AActor* Owner = GetOwner();
	USplineComponent* SplineComponent = Owner->FindComponentByClass<USplineComponent>();
	if (SplineComponent)
	{
		while (SplineComponent->GetNumberOfSplinePoints() > 0)
		{
			SplineComponent->RemoveSplinePoint(0, true);
		}
		SplineComponent->SetVisibility(bVisible, true);
	}
	if (!bVisible)
	{
		return;
	}
	if (SplineComponent && Solution.Num() > 0)
	{
		for (int32 i = 0; i < Solution.Num() - 1; ++i)
		{
			FVector SplineLocation = Solution[i];
			SplineLocation.Z += SplineZOffset;
			SplineComponent->AddSplinePoint(SplineLocation, ESplineCoordinateSpace::World);
		}
		FVector SplineLocation = GetComponentLocation();
		SplineLocation.Z += SplineZOffset;
		SplineComponent->AddSplinePoint(SplineLocation, ESplineCoordinateSpace::World);

		TArray<USceneComponent*> ComponentsInRoot;
		SplineComponent->GetChildrenComponents(false, ComponentsInRoot);
		for (USceneComponent* ComponentInRoot : ComponentsInRoot)
		{
			if (ComponentInRoot->IsA(USplineMeshComponent::StaticClass()))
			{
				ComponentInRoot->DestroyComponent();
				ComponentInRoot->SetActive(false);
			}
		}

		if (DebugNavigationMesh && DebugNavigationMaterial)
		{
			float MaxDistance = SplineComponent->GetSplineLength() - SplineInterval;
			for (float CurrentDistance = 0.f; CurrentDistance < MaxDistance - SplineInterval; CurrentDistance += SplineInterval)
			{
				FVector Location, Tangent, LocationNext, TangentNext;
				Location = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::Local);
				LocationNext = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance + SplineInterval, ESplineCoordinateSpace::Local);
				Tangent = SplineComponent->GetTangentAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::Local);
				TangentNext = SplineComponent->GetTangentAtDistanceAlongSpline(CurrentDistance + SplineInterval, ESplineCoordinateSpace::Local);

				USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
				SplineMeshComponent->bCastDynamicShadow = false;
				SplineMeshComponent->SetMobility(EComponentMobility::Movable);
				SplineMeshComponent->SetStartAndEnd(Location, Tangent, LocationNext, TangentNext);
				SplineMeshComponent->SetStartScale(FVector2D(.15f, .15f));
				SplineMeshComponent->SetEndScale(FVector2D(.35f, .35f));
				SplineMeshComponent->SetupAttachment(SplineComponent);
				SplineMeshComponent->RegisterComponent();

				SplineMeshComponent->SetStaticMesh(DebugNavigationMesh);

				UMaterialInstanceDynamic* MutableMaterial = UMaterialInstanceDynamic::Create(DebugNavigationMaterial, this);
				MutableMaterial->SetVectorParameterValue(FName(TEXT("Color")), DebugColor);

				SplineMeshComponent->SetMaterial(0, MutableMaterial);
			}
		}
	}
}

