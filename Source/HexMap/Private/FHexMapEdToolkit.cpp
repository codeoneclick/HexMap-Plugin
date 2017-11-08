// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"

#if WITH_EDITOR

#include "HexMap.h"
#include "FHexMapEdToolkit.h"
#include "FHexMapEdMode.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "SExpandableArea.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "HexMapEdModeProperties.h"
#include "EditorViewportClient.h"
#include "HMGrid.h"
#include "HMTile.h"
#include "HMUtilities.h"
#include "HMTileRandomizer.h"
#include "HMTileBatchApplier.h"

#define LOCTEXT_NAMESPACE "FHexMapEdToolkit"

FHexMapEdToolkit::FHexMapEdToolkit()
{
}

FHexMapEdToolkit::~FHexMapEdToolkit()
{
}

void FHexMapEdToolkit::Init(const TSharedPtr<class IToolkitHost>& ToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return true;
		}
	};

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Left)
		.Padding(16)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_SetTileSize_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_AddCircle_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_AddRectangle_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_AddTile_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_RandomizeTiles_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_TilesBatchApplier_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Center)
			[
				MAKE_Copyright_SLOT(this)
			]
		];
	FModeToolkit::Init(ToolkitHost);
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

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_Copyright_SLOT(FHexMapEdToolkit* SELF)
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(32.f)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.AutoWrapText(true)
			.Text(LOCTEXT("CopyrightLabel", "Copyright (c) 2017 Serhii Serhiiv. All rights reserved."))
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_SetTileSize_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelSetTileSize = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelSetTileSize->SetObject(HexMapEdMode->EdModePropertiesSetTileSize, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("SetHexMapTileSizeHeader", "SetHexMapTileSizeHeader", "Set Tile Size"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelSetTileSize.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_SetTileSize_BTN(LOCTEXT("SetHMTileSizeBTN", "Set Size"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_AddCircle_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelAddCircle = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelAddCircle->SetObject(HexMapEdMode->EdModePropertiesAddCircle, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("HM_HEADER_AddCircle", "HM_HEADER_AddCircle", "Add Circle"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelAddCircle.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_AddCircle_BTN(LOCTEXT("HM_BTN_AddCircle", "Add Circle"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_AddRectangle_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelAddRectangle = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelAddRectangle->SetObject(HexMapEdMode->EdModePropertiesAddRectangle, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("HM_HEADER_AddRectangle", "HM_HEADER_AddRectangle", "Add Rectangle"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelAddRectangle.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_AddRectangle_BTN(LOCTEXT("HM_BTN_AddRectangle", "Add Rectangle"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_AddTile_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelAddTile = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelAddTile->SetObject(HexMapEdMode->EdModePropertiesAddTile, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("AddTileHeader", "AddTileHeader", "Add Tile"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelAddTile.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_AddTile_BTN(LOCTEXT("AddHMTileBTN", "Add Tile"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_RandomizeTiles_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelRandomizeTiles = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelRandomizeTiles->SetObject(HexMapEdMode->EdModePropertiesRandomizeTiles, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("RandomizeTilesHeader", "RandomizeTilesHeader", "Randomize Tiles"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
			[
				SELF->EdModePanelRandomizeTiles.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.AutoHeight()
			[
				MAKE_RandomizeTiles_BTN(LOCTEXT("RandomizeHMTilesBTN", "Randomize"))
			]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_TilesBatchApplier_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->EdModePanelTilesBatchApplier = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->EdModePanelTilesBatchApplier->SetObject(HexMapEdMode->EdModePropertiesTileBatchApplier, true);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SSeparator)
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.BorderImage(FEditorStyle::GetBrush("ToolBar.Background"))
			.Padding(8.f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
				.Text(NSLOCTEXT("TilesBatchApplierHeader", "TilesBatchApplierHeader", "Tiles Batch Applier"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->EdModePanelTilesBatchApplier.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_TilesBatchApplier_BTN(LOCTEXT("HMTilesBatchApplierBTN", "Apply"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_SetTileSize_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_SetTileSize_BTN);
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_AddCircle_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_AddCircle_BTN);
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_AddRectangle_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_AddRectangle_BTN);
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_AddTile_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_AddTile_BTN);
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_RandomizeTiles_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_RandomizeTiles_BTN);
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_TilesBatchApplier_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_TilesBatchApplier_BTN);
}

FReply FHexMapEdToolkit::ON_AddCircle_BTN()
{
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
	if (HexMapEdMode->EdModePropertiesAddCircle->Tile_BP)
	{
		FVector Location = HexMapEdMode->EdModePropertiesAddCircle->Location;
		if (Location.IsZero())
		{
			FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
			FVector EditorCameraDirection = ViewportClient->GetViewRotation().Vector();
			FVector EditorCameraPosition = ViewportClient->GetViewLocation();
			float Distance = 3000.f;
			Location = EditorCameraPosition + EditorCameraDirection * Distance;
		}

		UWorld* World = GEditor->GetEditorWorldContext().World();

		GEditor->BeginTransaction(LOCTEXT("HM_TRANSITION_AddCircle", "HM_TRANSITION_AddCircle"));
		{
			GEditor->SelectNone(true, true);
			int32 Radius = HexMapEdMode->EdModePropertiesAddCircle->Radius;
			for (int32 Q = -Radius; Q <= Radius; ++Q)
			{
				int32 R1 = std::max(-Radius, -Q - Radius);
				int32 R2 = std::min(Radius, -Q + Radius);

				for (int32 R = R1; R <= R2; R++)
				{
					FHMCoord HexCoord = FHMCoord::Init(Q, R, -Q - R);
					FVector SnapLocation = FHMUtilities::ToSnapLocation(World, HexCoord);
					SnapLocation = FHMUtilities::ToSnapLocation(World, SnapLocation + Location);

					AHMTile* Tile = World->SpawnActor<AHMTile>(HexMapEdMode->EdModePropertiesAddCircle->Tile_BP, SnapLocation, FRotator(0.f));
					GEditor->SelectActor(Tile, true, true);
				}
			}
		}
		GEditor->EndTransaction();
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Tile Blueprint should be assigned!");
	}

	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_AddRectangle_BTN()
{
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
	if (HexMapEdMode->EdModePropertiesAddRectangle->Tile_BP)
	{
		FVector Location = HexMapEdMode->EdModePropertiesAddRectangle->Location;
		if (Location.IsZero())
		{
			FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
			FVector EditorCameraDirection = ViewportClient->GetViewRotation().Vector();
			FVector EditorCameraPosition = ViewportClient->GetViewLocation();
			float Distance = 3000.f;
			Location = EditorCameraPosition + EditorCameraDirection * Distance;
		}

		UWorld* World = GEditor->GetEditorWorldContext().World();

		GEditor->BeginTransaction(LOCTEXT("HM_TRANSITION_AddRectangle", "HM_TRANSITION_AddRectangle"));
		{
			GEditor->SelectNone(true, true);
			int32 SizeX = HexMapEdMode->EdModePropertiesAddRectangle->SizeX;
			int32 SizeY = HexMapEdMode->EdModePropertiesAddRectangle->SizeY;
			for (int32 i = 0; i < SizeX; ++i)
			{
				for (int32 j = 0; j < SizeY; ++j)
				{
					FHMCoord HexCoord = FHMCoord::QOffsetToCube(FHMCoord::EHMDirection::ODD, FVector2D(i, j));
					FVector SnapLocation = FHMUtilities::ToSnapLocation(World, HexCoord);
					SnapLocation = FHMUtilities::ToSnapLocation(World, SnapLocation + Location);

					AHMTile* Tile = World->SpawnActor<AHMTile>(HexMapEdMode->EdModePropertiesAddRectangle->Tile_BP, SnapLocation, FRotator(0.f));
					GEditor->SelectActor(Tile, true, true);
				}
			}
		}
		GEditor->EndTransaction();
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Tile Blueprint should be assigned!");
	}

	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_AddTile_BTN()
{
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
	if (HexMapEdMode->EdModePropertiesAddTile->Tile_BP)
	{
		FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
		FVector EditorCameraDirection = ViewportClient->GetViewRotation().Vector();
		FVector EditorCameraPosition = ViewportClient->GetViewLocation();
		float Distance = 3000.f;
		FVector Location = EditorCameraPosition + EditorCameraDirection * Distance;

		UWorld* World = GEditor->GetEditorWorldContext().World();

		GEditor->BeginTransaction(LOCTEXT("AddHMTile", "AddHMTile"));
		{
			GEditor->SelectNone(true, true);
			AHMTile* Tile = World->SpawnActor<AHMTile>(HexMapEdMode->EdModePropertiesAddTile->Tile_BP, Location, FRotator(0.f));
			GEditor->SelectActor(Tile, true, true);
		}
		GEditor->EndTransaction();
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Tile Blueprint should be assigned!");
	}
	
	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_RandomizeTiles_BTN()
{
	TArray<AHMTile*> SelectedTiles;
	GetSelectedTiles(SelectedTiles);
	if (SelectedTiles.Num() > 0)
	{
		FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
		if (HexMapEdMode && HexMapEdMode->EdModePropertiesRandomizeTiles->Randomizer_BP)
		{
			UWorld* World = GEditor->GetEditorWorldContext().World();
			GEditor->BeginTransaction(LOCTEXT("HM_TRANSITION_Randomize", "HM_TRANSITION_Randomize"));
			AHMTileRandomizer* Randomizer = World->SpawnActor<AHMTileRandomizer>(HexMapEdMode->EdModePropertiesRandomizeTiles->Randomizer_BP, FVector(0.f), FRotator(0.f));
			for (AHMTile* Tile : SelectedTiles)
			{
				Randomizer->Randomize(Tile);
			}
			Randomizer->Destroy();
			GEditor->EndTransaction();
		}
		else
		{
			GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "You need to choose Randomizer Blueprint at first!");
		}
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "At least one Tile should be selected!");
	}
	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_TilesBatchApplier_BTN()
{
	TArray<AHMTile*> SelectedTiles;
	GetSelectedTiles(SelectedTiles);
	if (SelectedTiles.Num() > 0)
	{
		FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
		if (HexMapEdMode && HexMapEdMode->EdModePropertiesTileBatchApplier->Applier_BP)
		{
			UWorld* World = GEditor->GetEditorWorldContext().World();
			GEditor->BeginTransaction(LOCTEXT("HMTilesBatchApplier", "HMTilesBatchApplier"));
			AHMTileBatchApplier* Applier = World->SpawnActor<AHMTileBatchApplier>(HexMapEdMode->EdModePropertiesTileBatchApplier->Applier_BP, FVector(0.f), FRotator(0.f));
			for (AHMTile* Tile : SelectedTiles)
			{
				Applier->Apply(Tile);
			}
			Applier->Destroy();
			GEditor->EndTransaction();
		}
		else
		{
			GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "You need to choose Applier Blueprint at first!");
		}
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "At least one HexMapTile should be selected!");
	}
	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_SetTileSize_BTN()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	GEditor->BeginTransaction(LOCTEXT("SetHMTileSize", "SetHMTileSize"));
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
	AHMGrid* Grid = FHMUtilities::GetGrid(World);
	Grid->OnTileSizeChanged(HexMapEdMode->EdModePropertiesSetTileSize->TileSize);
	GEditor->EndTransaction();
	return FReply::Handled();
}

void FHexMapEdToolkit::GetSelectedTiles(TArray<class AHMTile*>& Tiles)
{
	USelection* Selection = GEditor->GetSelectedActors();
	for (int32 i = 0; i < Selection->Num(); ++i)
	{
		UObject* Object = Selection->GetSelectedObject(i);
		if (Object && Object->IsA(AHMTile::StaticClass()))
		{
			Tiles.Add(Cast<AHMTile>(Object));
		}
	}
}

#endif
