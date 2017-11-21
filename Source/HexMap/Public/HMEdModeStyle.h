// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "ISettingsModule.h"
#include "ModuleManager.h"

class FHMEdModeStyle
{
protected:

	static FString InContent(const FString& RelativePath, const ANSICHAR* Extension);
	static TSharedPtr< class FSlateStyleSet > StyleSet;

public:

	FHMEdModeStyle() = default;
	~FHMEdModeStyle() = default;

	static void Initialize();
	static void Shutdown();
	static TSharedPtr<class ISlateStyle> Get();
};

#endif