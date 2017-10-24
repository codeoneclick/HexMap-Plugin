#pragma once
 
#include "ModuleManager.h"
 
class HexMapImpl : public IModuleInterface
{
public:

	void StartupModule();
	void ShutdownModule();
};