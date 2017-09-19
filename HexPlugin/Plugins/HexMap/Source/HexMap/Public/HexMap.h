#pragma once
 
#include "ModuleManager.h"
 
class HexMapImpl : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();
};