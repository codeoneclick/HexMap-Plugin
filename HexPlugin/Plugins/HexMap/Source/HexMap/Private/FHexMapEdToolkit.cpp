// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "FHexMapEdToolkit.h"
#include "FHexMapEdMode.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"

FHexMapEdToolkit::FHexMapEdToolkit()
{
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
