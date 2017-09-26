// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/UnrealEd/Public/Toolkits/BaseToolkit.h"

class FHexMapEdToolkit : public FModeToolkit
{
protected:

	TSharedPtr<SWidget> ToolkitWidget;

public:
	FHexMapEdToolkit();
	~FHexMapEdToolkit();

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }
};
