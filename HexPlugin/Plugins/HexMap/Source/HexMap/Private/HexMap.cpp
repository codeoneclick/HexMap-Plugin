#include "HexMapPrivatePCH.h"
#include "HexMap.h"
 
void HexMapImpl::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("HexMap Plugin loaded!"))
}
 
void HexMapImpl::ShutdownModule()
{
    
}
 
IMPLEMENT_MODULE(HexMapImpl, Module)
