
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlatformDataLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMDATA_API UPlatformDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** Возращает кол-во используемой видиопамити. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (KeyWords = "VRAM"), Category = PlatformData)
	static int32 GetUsedVRAM();

	/** Возращает память текущей видиокарты. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (KeyWords = "VRAM"), Category = PlatformData)
	static int32 GetAvailableVRAM();

	/** Возращает название бренда графического процессора. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get GPU Brand Name", Keywords = "GPU brand"), Category = PlatformData)
	static FString GetGPUBrandName();

	/** Возращает названия доступных мониторов. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Monitors", Keywords = "Display"), Category = PlatformData)
	static TArray<FString> GetMonitors();

	/** Возращает текущее аудиосистему. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Audio Device Name", Keywords = "Audio device", WorldContext = "WorldContext"), Category = PlatformFunctions)
	static FString GetAudioDeviceName(const UObject* WorldContext);

	/** Возращает все аудиосистемы. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Audio Devices Name", Keywords = "Audio device"), Category = PlatformFunctions)
	static TArray<FString> GetAudioDevicesName();

	/** Возращает кол-во мониторов. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Monitors Count", Keywords = "Display"), Category = PlatformData)
	static int32 GetMonitorsCount();

	/** Возращает индекс основного монитора. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Primary Monitor", Keywords = "Display"), Category = PlatformData)
	static int32 GetPrimaryMonitor();

	/** Устанавливает новый активный монитор. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Active Monitor", Keywords = "Display"), Category = PlatformData)
	static bool SetActiveMonitor(int32 MonitorIndex);

	/** Устанавливает гамму игры. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Gamma", Keywords = "Display"), Category = PlatformData)
	static void SetGamma(float Gamma);

	/** Возращает гамму игры. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Display"), Category = PlatformData)
	static float GetGamma();
};
