
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InitializableWidgetInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInitializableWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Интерфейс иницилизации для виджетов помещенных в CustomWidgetComponent.
 */
class CALYPSO_DEV_API IInitializableWidgetInterface
{
	GENERATED_BODY()

public:
	/** Вызывается когда виджет был создан в WidgetComponent. */
	UFUNCTION(BlueprintNativeEvent, meta = (Category = "Init"))
	void OnCreatedWidget(class UCustomWidgetComponent* InWidgetComponent);
};
