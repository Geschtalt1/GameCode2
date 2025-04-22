
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
	/** ��������� ���-�� ������������ �����������. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (KeyWords = "VRAM"), Category = PlatformData)
	static int32 GetUsedVRAM();

	/** ��������� ������ ������� ����������. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (KeyWords = "VRAM"), Category = PlatformData)
	static int32 GetAvailableVRAM();

	/** ��������� �������� ������ ������������ ����������. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get GPU Brand Name", Keywords = "GPU brand"), Category = PlatformData)
	static FString GetGPUBrandName();

	/** ��������� �������� ��������� ���������. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Monitors", Keywords = "Display"), Category = PlatformData)
	static TArray<FString> GetMonitors();

	/** ��������� ������� ������������. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Audio Device Name", Keywords = "Audio device", WorldContext = "WorldContext"), Category = PlatformFunctions)
	static FString GetAudioDeviceName(const UObject* WorldContext);

	/** ��������� ��� ������������. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Audio Devices Name", Keywords = "Audio device"), Category = PlatformFunctions)
	static TArray<FString> GetAudioDevicesName();

	/** ��������� ���-�� ���������. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Monitors Count", Keywords = "Display"), Category = PlatformData)
	static int32 GetMonitorsCount();

	/** ��������� ������ ��������� ��������. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Primary Monitor", Keywords = "Display"), Category = PlatformData)
	static int32 GetPrimaryMonitor();

	/** ������������� ����� �������� �������. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Active Monitor", Keywords = "Display"), Category = PlatformData)
	static bool SetActiveMonitor(int32 MonitorIndex);

	/** ������������� ����� ����. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Gamma", Keywords = "Display"), Category = PlatformData)
	static void SetGamma(float Gamma);

	/** ��������� ����� ����. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Display"), Category = PlatformData)
	static float GetGamma();
};
