// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"

#if WITH_EDITOR

#include "HexMap.h"
#include "FHexMapEdToolkit.h"
#include "FHexMapEdMode.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"
#include "HexMapChunkActor.h"
#include "HexMapGeneralActor.h"
#include "HexMapTileObjectActor.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"

#define LOCTEXT_NAMESPACE "FHexMapEdToolkit"

FHexMapEdToolkit::FHexMapEdToolkit()
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return true;
		}

		static FReply OnHexMapClearButtonClick()
		{
			if (EAppReturnType::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("HexMapTilesFillMessage", "All previous attached tiles will be removed. Are you sure?")))
			{
				FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
				UWorld* World = GEditor->GetEditorWorldContext().World();
				for (TActorIterator<AHexMapTileObjectActor> HexMapTileObjectActorItr(World); HexMapTileObjectActorItr; ++HexMapTileObjectActorItr)
				{
					AHexMapTileObjectActor* HexMapTileObjectActor = *HexMapTileObjectActorItr;
					HexMapTileObjectActor->Destroy();
				}
			}
			return FReply::Handled();
		}

		static FReply OnHexMapFillButtonClick()
		{
			if (EAppReturnType::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("HexMapTilesFillMessage", "All previous attached tiles will be removed. Are you sure?")))
			{
				FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
				UWorld* World = GEditor->GetEditorWorldContext().World();
				for (TActorIterator<AHexMapTileObjectActor> HexMapTileObjectActorItr(World); HexMapTileObjectActorItr; ++HexMapTileObjectActorItr)
				{
					AHexMapTileObjectActor* HexMapTileObjectActor = *HexMapTileObjectActorItr;
					HexMapTileObjectActor->Destroy();
				}
				for (TActorIterator<AHexMapGeneralActor> HexMapGeneralActorItr(World); HexMapGeneralActorItr; ++HexMapGeneralActorItr)
				{
					AHexMapGeneralActor* HexMapGeneralActor = *HexMapGeneralActorItr;
					auto HexMapChunkActorToPositionLinkages = HexMapGeneralActor->HexMapChunkActorToPositionLinkage;
					for (auto HexMapChunkActorToPositionLinkage : HexMapChunkActorToPositionLinkages)
					{
						FVector Location = HexMapChunkActorToPositionLinkage.Key;
						FRotator Rotation(0.0f, 0.0f, 0.0f);
						FActorSpawnParameters SpawnInfo;
						AHexMapTileObjectActor* HexMapTileObjectActor = World->SpawnActor<AHexMapTileObjectActor>(Location, Rotation, SpawnInfo);
						HexMapTileObjectActor->AttachToActor(HexMapChunkActorToPositionLinkage.Value, AttachmentTransformRules);
					}
					for (TActorIterator<ANavMeshBoundsVolume> HexMapNavigationMeshVolumeItr(World); HexMapNavigationMeshVolumeItr; ++HexMapNavigationMeshVolumeItr)
					{
						//ANavMeshBoundsVolume* HexMapNavigationMeshVolume = *HexMapNavigationMeshVolumeItr;
						//HexMapNavigationMeshVolume->AttachToActor();
					}
					/*FVector Location(0.f, 0.f, 0.f);
					FRotator Rotation(0.0f, 0.0f, 0.0f);
					FActorSpawnParameters SpawnInfo;
					ANavMeshBoundsVolume* HexMapNavigationMeshVolume = World->SpawnActor<ANavMeshBoundsVolume>(Location, Rotation, SpawnInfo);*/
					break;
				}
			}
			return FReply::Handled();
		}

		static FReply OnAttachHexMapChunksToGeneralButtonClick()
		{
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
			UWorld* World = GEditor->GetEditorWorldContext().World();
			for (TActorIterator<AHexMapGeneralActor> HexMapGeneralActorItr(World); HexMapGeneralActorItr; ++HexMapGeneralActorItr)
			{
				AHexMapGeneralActor* HexMapGeneralActor = *HexMapGeneralActorItr;
				TArray<AHexMapChunkActor *> HexMapChunkActors;
				for (TActorIterator<AHexMapChunkActor> HexMapChunkActorItr(World); HexMapChunkActorItr; ++HexMapChunkActorItr)
				{
					AHexMapChunkActor* HexMapChunkActor = *HexMapChunkActorItr;
					if (!HexMapChunkActor->IsAttachedTo(HexMapGeneralActor))
					{
						HexMapChunkActor->AttachToActor(HexMapGeneralActor, AttachmentTransformRules);
					}
					HexMapChunkActors.Add(HexMapChunkActor);
				}
				HexMapGeneralActor->HexMapChunkActors = HexMapChunkActors;
				HexMapGeneralActor->OnHexMapChunkAttachesChanged();
				break;
			}
			return FReply::Handled();
		}

		static FReply OnHexMapTilesRefreshPositionsButtonClick()
		{
			UWorld* World = GEditor->GetEditorWorldContext().World();
			for (TActorIterator<AHexMapGeneralActor> HexMapGeneralActorItr(World); HexMapGeneralActorItr; ++HexMapGeneralActorItr)
			{
				AHexMapGeneralActor* HexMapGeneralActor = *HexMapGeneralActorItr;
				HexMapGeneralActor->OnHexMapChunkActorChangedLocation();
			}
			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeAttachHexMapChunksToGeneralButton(FText Label)
		{
			return SNew(SButton)
				.Text(Label)
				.OnClicked_Static(&Locals::OnAttachHexMapChunksToGeneralButtonClick);
		}

		static TSharedRef<SWidget> MakeHexMapTilesRefreshPositionsButton(FText Label)
		{
			return SNew(SButton)
				.Text(Label)
				.OnClicked_Static(&Locals::OnHexMapTilesRefreshPositionsButtonClick);
		}

		static TSharedRef<SWidget> MakeHexMapFillButton(FText Label)
		{
			return SNew(SButton)
				.Text(Label)
				.OnClicked_Static(&Locals::OnHexMapFillButtonClick);
		}

		static TSharedRef<SWidget> MakeHexMapClearButton(FText Label)
		{
			return SNew(SButton)
				.Text(Label)
				.OnClicked_Static(&Locals::OnHexMapClearButtonClick);
		}
	};

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Left)
		.Padding(16)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(0)
		[
			SNew(STextBlock)
			.AutoWrapText(true)
		.Text(LOCTEXT("CopyrightLabel", "Copyright (c) 2017 Serhii Serhiiv. All rights reserved."))
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		.Padding(8)
		[
			Locals::MakeAttachHexMapChunksToGeneralButton(LOCTEXT("AttachHexMapChunksToGeneral", "Attach HexMap Chunks To General"))
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		.Padding(8)
		[
			Locals::MakeHexMapTilesRefreshPositionsButton(LOCTEXT("HexMapTilesRefreshPositions", "Refresh Tiles Positions"))
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		.Padding(8)
		[
			Locals::MakeHexMapFillButton(LOCTEXT("HexMapTilesFill", "Fill HexMap"))
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		.Padding(8)
		[
			Locals::MakeHexMapClearButton(LOCTEXT("HexMapTilesClear", "Clear HexMap"))
		]

		];
}

FHexMapEdToolkit::~FHexMapEdToolkit()
{
}

FName FHexMapEdToolkit::GetToolkitFName() const
{
	return FName("HexMapEdToolkit");
}

FText FHexMapEdToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("HexMapEdToolkit", "DisplayName", "HexMapEd Tool");
}

class FEdMode* FHexMapEdToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap);
}

#endif
