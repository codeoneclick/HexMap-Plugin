// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "FHexMapEdToolkit.h"
#include "FHexMapEdMode.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FHexMapEdToolkit"

FHexMapEdToolkit::FHexMapEdToolkit()
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return true;
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}
	};

	const float Factor = 256.0f;

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
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
		.Text(LOCTEXT("HelperLabel", "Select some actors and move 231231 them around using buttons below"))
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.AutoHeight()
		[
			Locals::MakeButton(LOCTEXT("UpButtonLabel", "Up"), FVector(0, 0, Factor))
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			Locals::MakeButton(LOCTEXT("LeftButtonLabel", "Left"), FVector(0, -Factor, 0))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			Locals::MakeButton(LOCTEXT("RightButtonLabel", "Right"), FVector(0, Factor, 0))
		]
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.AutoHeight()
		[
			Locals::MakeButton(LOCTEXT("DownButtonLabel", "Down"), FVector(0, 0, -Factor))
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
