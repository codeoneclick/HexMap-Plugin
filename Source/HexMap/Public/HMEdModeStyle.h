// Copyright serhii serhiiv 2017. All rights reserved.

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
