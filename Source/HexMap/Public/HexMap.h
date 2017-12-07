// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once
 
#include "ModuleManager.h"
 
class HexMapImpl : public IModuleInterface
{
public:

	void StartupModule();
	void ShutdownModule();
};