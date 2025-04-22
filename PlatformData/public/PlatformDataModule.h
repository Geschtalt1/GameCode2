#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Windows/WindowsHWrapper.h"

THIRD_PARTY_INCLUDES_START
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include "dxgi1_4.h"
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_END

class FPlatformData : public FDefaultModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	/** ¬озращает используемую видеопам€ть текущей видиокарты. */
	static PLATFORMDATA_API size_t GetUsedVideoMemory();

	/** ¬озращает пам€ть текущей видиокарты. */
	static PLATFORMDATA_API size_t GetAvailableVideoMemory();
};