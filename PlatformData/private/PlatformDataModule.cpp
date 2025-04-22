#include "PlatformDataModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FPlatformData, PlatformData);

void FPlatformData::StartupModule()
{
}

void FPlatformData::ShutdownModule()
{
}

PLATFORMDATA_API size_t FPlatformData::GetUsedVideoMemory()
{
    IDXGIFactory4* pFactory;
    CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pFactory);

    IDXGIAdapter3* adapter;
    pFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&adapter));

    DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
    adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);

    size_t usedVRAM = videoMemoryInfo.CurrentUsage / 1024 / 1024;

    return usedVRAM;
}

PLATFORMDATA_API size_t FPlatformData::GetAvailableVideoMemory()
{
    // Создаем фабрику.
    IDXGIFactory4* pFactory;
    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pFactory);

    // Проверяем что фабрика создана без ошибок.
    if (FAILED(hr) != true)
    {
        // Получаем адаптер по индексу.
        IDXGIAdapter1* pAdapter;
        pFactory->EnumAdapters1(0, &pAdapter);

        DXGI_ADAPTER_DESC1 desc;
        pAdapter->GetDesc1(&desc);

        // Возращает память в мегабайтах.
        return desc.DedicatedVideoMemory / (1024.0f * 1024.0f);
    }

    return size_t();
}
