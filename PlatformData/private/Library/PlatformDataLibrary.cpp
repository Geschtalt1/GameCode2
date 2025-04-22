

#include "Library/PlatformDataLibrary.h"
#include "PlatformDataModule.h"
#include "Engine.h"
#include "AudioDevice.h"
#include "HAL/IConsoleManager.h"
#include "Misc/CommandLine.h"
#include "Engine/GameViewportClient.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"
#include "AudioMixerDevice.h"

int32 UPlatformDataLibrary::GetUsedVRAM()
{
	return static_cast<int32>(FPlatformData::GetUsedVideoMemory());
}

int32 UPlatformDataLibrary::GetAvailableVRAM()
{
	return static_cast<int32>(FPlatformData::GetAvailableVideoMemory());
}

TArray<FString> UPlatformDataLibrary::GetMonitors()
{
	FDisplayMetrics Display;
	FDisplayMetrics::RebuildDisplayMetrics(Display);

	TArray<FString> AllDisplays;
	for (auto& DisplayLocal : Display.MonitorInfo)
	{
		AllDisplays.Add(DisplayLocal.Name);
	}

	return AllDisplays;
}

FString UPlatformDataLibrary::GetAudioDeviceName(const UObject* WorldContext)
{
	auto AudioMixerDevice = GEngine->GetAudioDeviceManager()->GetAudioMixerDeviceFromWorldContext(WorldContext);
	if (AudioMixerDevice)
	{
		Audio::IAudioMixerPlatformInterface* MixerPlatform = AudioMixerDevice->GetAudioMixerPlatform();
		if (MixerPlatform)
		{
			return MixerPlatform->GetCurrentDeviceName();
		}
	}

	return "";
}

TArray<FString> UPlatformDataLibrary::GetAudioDevicesName()
{
	TArray<FString> DeviceNames;

	if (GEngine)
	{
		TArray<FAudioDevice*> AudioDevices = GEngine->GetAudioDeviceManager()->GetAudioDevices();

		if (AudioDevices.Num() > 0)
		{
			AudioDevices[0]->GetAudioDeviceList(DeviceNames);
		}
	}

	return DeviceNames;
}

int32 UPlatformDataLibrary::GetMonitorsCount()
{
	FDisplayMetrics Display;
	FDisplayMetrics::RebuildDisplayMetrics(Display);

	return Display.MonitorInfo.Num();
}

int32 UPlatformDataLibrary::GetPrimaryMonitor()
{
	FDisplayMetrics Display;
	FDisplayMetrics::RebuildDisplayMetrics(Display);

	int32 MonitorNumber = 0;
	FParse::Value(FCommandLine::Get(), TEXT("monitor="), MonitorNumber);

	int32 MonitorIndex = INDEX_NONE;
	if (MonitorNumber == 0)
	{
		for (int32 Index = 0; Index < Display.MonitorInfo.Num(); Index++)
		{
			if (Display.MonitorInfo[Index].bIsPrimary)
			{
				MonitorIndex = Index;
				break;
			}
		}
	}
	else
	{
		MonitorIndex = MonitorNumber - 1;
	}

	return MonitorIndex;
}

bool UPlatformDataLibrary::SetActiveMonitor(int32 MonitorIndex)
{
	if (MonitorIndex > GetMonitorsCount() - 1)
		return false;

	if (!GEngine)
		return false;

	if (!GEngine->GameViewport)
		return false;

	FDisplayMetrics Display;
	FDisplayMetrics::RebuildDisplayMetrics(Display);

	if (!Display.MonitorInfo.IsValidIndex(MonitorIndex))
		return false;

	const FMonitorInfo Monitor = Display.MonitorInfo[MonitorIndex];

	// (can be implemented in Blueprints)
	// Check if used monitor resolution is bigger then selected new monitor resolution if so change game resolution to selected monitor resolution
	// check if UserSettings.ResolutionSizeX > Monitor.NativeWidth || UserSettings.ResolutionSizeY > Monitor.NativeHeight
	// if true then change your game resolution to Monitor.NativeWidth, Monitor.NativeHeight

	const int32 WindowPosX = Monitor.WorkArea.Left;
	const int32 WindowPosY = Monitor.WorkArea.Top;
	const FVector2D Position(static_cast<float>(WindowPosX), static_cast<float>(WindowPosY));

	TSharedPtr<SWindow> Window = GEngine->GameViewport->GetWindow();

	// Best to set window position in WindowMode for Linux
#if PLATFORM_LINUX
	const EWindowMode::Type CurrentWindowMode = Window->GetWindowMode();
	Window->SetWindowMode(EWindowMode::Windowed);
#endif

	Window->MoveWindowTo(Position);

#if PLATFORM_LINUX
	// Set WindowMode back to mode before MoveWindow
	Window->SetWindowMode(CurrentWindowMode);
#endif

	return true;
}

void UPlatformDataLibrary::SetGamma(float Gamma)
{
	GEngine->DisplayGamma = FMath::Clamp(Gamma, 1.0f, 5.0f);
}

float UPlatformDataLibrary::GetGamma()
{
	return GEngine->GetDisplayGamma();
}

FString UPlatformDataLibrary::GetGPUBrandName()
{
	return FPlatformMisc::GetPrimaryGPUBrand();
}
