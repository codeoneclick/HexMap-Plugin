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
#include "HexMapGrid.h"
#include "HexMapChunk.h"
#include "HexUtils.h"
#include "HexMapCircleChunk.h"
#include "HexMapRectangleChunk.h"
#include "HexMapTile.h"
#include "HexMapTileLocationComponent.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "HexMapEdModeProperties.h"
#include "EditorViewportClient.h"
#include "HexMapTileRandomizer.h"

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
				MAKE_CreateCircleChunk_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_CreateRectangleChunk_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_FillChunks_SLOT(this)
			]
			+ SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				MAKE_ClearChunks_SLOT(this)
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
	SELF->HexMapEdModeSetTileSizePanel = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->HexMapEdModeSetTileSizePanel->SetObject(HexMapEdMode->EdModeSetTileSizeProperties, true);
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
					SELF->HexMapEdModeSetTileSizePanel.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_SetTileSize_BTN(LOCTEXT("SetHMTileSizeBTN", "Execute"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_CreateCircleChunk_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->HexMapEdModeCreateCircleChunkPanel = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->HexMapEdModeCreateCircleChunkPanel->SetObject(HexMapEdMode->EdModeCreateCircleChunkProperties, true);
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
				.Text(NSLOCTEXT("CreateHexMapCircleChunkHeader", "CreateHexMapCircleChunkHeader", "Create Circle Chunk"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->HexMapEdModeCreateCircleChunkPanel.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_CreateCircleChunk_BTN(LOCTEXT("CreateHMChunkBTN", "Execute"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_CreateRectangleChunk_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->HexMapEdModeCreateRectangleChunkPanel = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->HexMapEdModeCreateRectangleChunkPanel->SetObject(HexMapEdMode->EdModeCreateRectangleChunkProperties, true);
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
				.Text(NSLOCTEXT("CreateHexMapRectangleChunkHeader", "CreateHexMapRectangleChunkHeader", "Create Rectangle Chunk"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->HexMapEdModeCreateRectangleChunkPanel.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_CreateRectangleChunk_BTN(LOCTEXT("CreateHMChunkBTN", "Execute"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_FillChunks_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->HexMapEdModeFillSelectedChunksPanel = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->HexMapEdModeFillSelectedChunksPanel->SetObject(HexMapEdMode->EdModeFillSelectedChunksProperties, true);
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
			.Text(NSLOCTEXT("FillSelectedChunksHeader", "FillSelectedChunksHeader", "Fill Selected Chunks"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				.Padding(8.f)
				[
					SELF->HexMapEdModeFillSelectedChunksPanel.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_FillSelectedChunks_BTN(LOCTEXT("FillHMChunksBTN", "Execute"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_ClearChunks_SLOT(FHexMapEdToolkit* SELF)
{
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
				.Text(NSLOCTEXT("ClearSelectedChunksHeader", "ClearSelectedChunksHeader", "Clear Selected Chunks"))
			]
			.BodyContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_ClearSelectedChunks_BTN(LOCTEXT("ClearHMChunksBTN", "Execute"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_AddTile_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->HexMapEdModeAddTilePanel = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->HexMapEdModeAddTilePanel->SetObject(HexMapEdMode->EdModeAddTileProperties, true);
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
					SELF->HexMapEdModeAddTilePanel.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					MAKE_AddTile_BTN(LOCTEXT("AddHMTileBTN", "Execute"))
				]
			]
		];
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_RandomizeTiles_SLOT(FHexMapEdToolkit* SELF)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea);
	SELF->HexMapEdModeRandomizeTilesPanel = PropertyEditorModule.CreateDetailView(Args);
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)SELF->GetEditorMode();
	if (HexMapEdMode)
	{
		SELF->HexMapEdModeRandomizeTilesPanel->SetObject(HexMapEdMode->EdModeRandomizeTilesProperties, true);
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
				SELF->HexMapEdModeRandomizeTilesPanel.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.AutoHeight()
			[
				MAKE_RandomizeTiles_BTN(LOCTEXT("RandomizeHMTilesBTN", "Execute"))
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

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_CreateCircleChunk_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_CreateCircleChunk_BTN);
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_CreateRectangleChunk_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_CreateRectangleChunk_BTN);
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_FillSelectedChunks_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_FillSelectedChunks_BTN);
}

TSharedRef<SWidget> FHexMapEdToolkit::MAKE_ClearSelectedChunks_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_ClearSelectedChunks_BTN);
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

TSharedRef<SWidget>FHexMapEdToolkit::MAKE_RandomizeTiles_BTN(const FText& Label)
{
	return SNew(SButton)
		.Text(Label)
		.ButtonColorAndOpacity(FLinearColor(.12f, .12f, .12f, 1.f))
		.ForegroundColor(FLinearColor::White)
		.HAlign(HAlign_Center)
		.OnClicked_Static(FHexMapEdToolkit::ON_RandomizeTiles_BTN);
}

FReply FHexMapEdToolkit::ON_CreateCircleChunk_BTN()
{
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
	FVector Location = HexMapEdMode->EdModeCreateCircleChunkProperties->Location;
	if (Location.IsZero())
	{
		FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
		FVector EditorCameraDirection = ViewportClient->GetViewRotation().Vector();
		FVector EditorCameraPosition = ViewportClient->GetViewLocation();
		float Distance = 3000.f;
		Location = EditorCameraPosition + EditorCameraDirection * Distance;
	}
	
	UWorld* World = GEditor->GetEditorWorldContext().World();

	GEditor->BeginTransaction(LOCTEXT("CreateHMCircleChunk", "CreateHMCircleChunk"));
	{
		AHexMapCircleChunk* Chunk = World->SpawnActor<AHexMapCircleChunk>(Location, FRotator(0.f));
		Chunk->SetRadius(HexMapEdMode->EdModeCreateCircleChunkProperties->Radius);
		GEditor->SelectNone(true, true);
		GEditor->SelectActor(Chunk, true, true);

		if (HexMapEdMode->EdModeCreateCircleChunkProperties->Tile_BP)
		{
			TArray<USceneComponent*> ComponentsInRoot;
			Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
			for (USceneComponent* ComponentInRoot : ComponentsInRoot)
			{
				if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
				{
					UHexMapTileLocationComponent* TileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
					if (TileLocationComponent->LinkedTile)
					{
						TileLocationComponent->LinkedTile->Destroy();
						TileLocationComponent->LinkedTile = nullptr;
					}
					AHexMapTile* Tile = World->SpawnActor<AHexMapTile>(HexMapEdMode->EdModeCreateCircleChunkProperties->Tile_BP, TileLocationComponent->GetComponentLocation(), FRotator(0.f));
				}
			}
		}
	}
	GEditor->EndTransaction();

	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_CreateRectangleChunk_BTN()
{
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
	FVector Location = HexMapEdMode->EdModeCreateRectangleChunkProperties->Location;
	if (Location.IsZero())
	{
		FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
		FVector EditorCameraDirection = ViewportClient->GetViewRotation().Vector();
		FVector EditorCameraPosition = ViewportClient->GetViewLocation();
		float Distance = 3000.f;
		Location = EditorCameraPosition + EditorCameraDirection * Distance;
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();

	GEditor->BeginTransaction(LOCTEXT("CreateHMRectangleChunk", "CreateHMRectangleChunk"));
	{
		AHexMapRectangleChunk* Chunk = World->SpawnActor<AHexMapRectangleChunk>(Location, FRotator(0.f));
		Chunk->SetSize(HexMapEdMode->EdModeCreateRectangleChunkProperties->SizeX, HexMapEdMode->EdModeCreateRectangleChunkProperties->SizeY);
		GEditor->SelectNone(true, true);
		GEditor->SelectActor(Chunk, true, true);

		if (HexMapEdMode->EdModeCreateRectangleChunkProperties->Tile_BP)
		{
			TArray<USceneComponent*> ComponentsInRoot;
			Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
			for (USceneComponent* ComponentInRoot : ComponentsInRoot)
			{
				if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
				{
					UHexMapTileLocationComponent* TileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
					if (TileLocationComponent->LinkedTile)
					{
						TileLocationComponent->LinkedTile->Destroy();
						TileLocationComponent->LinkedTile = nullptr;
					}
					AHexMapTile* Tile = World->SpawnActor<AHexMapTile>(HexMapEdMode->EdModeCreateRectangleChunkProperties->Tile_BP, TileLocationComponent->GetComponentLocation(), FRotator(0.f));
				}
			}
		}
	}
	GEditor->EndTransaction();

	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_FillSelectedChunks_BTN()
{
	TArray<AHexMapChunk *> SelectedChunks;
	GetSelectedChunks(SelectedChunks);
	if (SelectedChunks.Num() > 0)
	{
		FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
		if (HexMapEdMode && HexMapEdMode->EdModeFillSelectedChunksProperties->Tile_BP)
		{
			if (EAppReturnType::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("HexMapTilesFillMessage", "All previous attached tiles will be removed. Are you sure?")))
			{
				UWorld* World = GEditor->GetEditorWorldContext().World();
				GEditor->BeginTransaction(LOCTEXT("FillHMChunks", "FillHMChunks"));
				for (AHexMapChunk* Chunk : SelectedChunks)
				{
					TArray<USceneComponent*> ComponentsInRoot;
					Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
					for (USceneComponent* ComponentInRoot : ComponentsInRoot)
					{
						if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
						{
							UHexMapTileLocationComponent* TileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
							if (TileLocationComponent->LinkedTile)
							{
								TileLocationComponent->LinkedTile->Destroy();
								TileLocationComponent->LinkedTile = nullptr;
							}
							AHexMapTile* Tile = World->SpawnActor<AHexMapTile>(HexMapEdMode->EdModeFillSelectedChunksProperties->Tile_BP, TileLocationComponent->GetComponentLocation(), FRotator(0.f));
						}
					}
				}
				GEditor->EndTransaction();
			}
		}
		else
		{
			GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "You need to choose Tile Blueprint at first!");
		}
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "At least one HexMapChunk should be selected!");
	}
	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_ClearSelectedChunks_BTN()
{
	TArray<AHexMapChunk *> SelectedChunks;
	GetSelectedChunks(SelectedChunks);
	if (SelectedChunks.Num() > 0)
	{
		if (EAppReturnType::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("HexMapTilesFillMessage", "All previous attached tiles will be removed. Are you sure?")))
		{
			GEditor->BeginTransaction(LOCTEXT("ClearHMChunks", "ClearHMChunks"));
			for (AHexMapChunk* Chunk : SelectedChunks)
			{
				TArray<USceneComponent*> ComponentsInRoot;
				Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
				for (USceneComponent* ComponentInRoot : ComponentsInRoot)
				{
					if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
					{
						UHexMapTileLocationComponent* TileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
						if (TileLocationComponent->LinkedTile)
						{
							TileLocationComponent->LinkedTile->Destroy();
							TileLocationComponent->LinkedTile = nullptr;
						}
					}
				}
			}
			GEditor->EndTransaction();
		}
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "At least one HexMapChunk should be selected!");
	}
	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_AddTile_BTN()
{
	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_RandomizeTiles_BTN()
{
	TArray<AHexMapChunk *> SelectedChunks;
	GetSelectedChunks(SelectedChunks);
	if (SelectedChunks.Num() > 0)
	{
		FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
		if (HexMapEdMode && HexMapEdMode->EdModeRandomizeTilesProperties->Randomizer_BP)
		{
			UWorld* World = GEditor->GetEditorWorldContext().World();
			GEditor->BeginTransaction(LOCTEXT("RandomizeHMTiles", "RandomizeHMTiles"));
			AHexMapTileRandomizer* Randomizer = World->SpawnActor<AHexMapTileRandomizer>(HexMapEdMode->EdModeRandomizeTilesProperties->Randomizer_BP, FVector(0.f), FRotator(0.f));
			for (AHexMapChunk* Chunk : SelectedChunks)
			{
				TArray<USceneComponent*> ComponentsInRoot;
				Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
				for (USceneComponent* ComponentInRoot : ComponentsInRoot)
				{
					if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
					{
						UHexMapTileLocationComponent* TileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
						if (TileLocationComponent->LinkedTile)
						{
							Randomizer->Randomize(TileLocationComponent->LinkedTile);
						}
					}
				}
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
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "At least one HexMapChunk should be selected!");
	}
	return FReply::Handled();
}

FReply FHexMapEdToolkit::ON_SetTileSize_BTN()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	GEditor->BeginTransaction(LOCTEXT("SetHMTileSize", "SetHMTileSize"));
	FHexMapEdMode* HexMapEdMode = (FHexMapEdMode*)(GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap));
	AHexMapGrid* Grid = FHex::GetGrid(World);
	Grid->SetTileSize(HexMapEdMode->EdModeSetTileSizeProperties->TileSize);
	GEditor->EndTransaction();
	return FReply::Handled();
}

void FHexMapEdToolkit::GetSelectedChunks(TArray<AHexMapChunk *>& Chunks)
{
	USelection* Selection = GEditor->GetSelectedActors();
	for (int32 i = 0; i < Selection->Num(); ++i)
	{
		UObject* Object = Selection->GetSelectedObject(i);
		if (Object && Object->IsA(AHexMapChunk::StaticClass()))
		{
			Chunks.Add(Cast<AHexMapChunk>(Object));
		}
	}
}

#endif
