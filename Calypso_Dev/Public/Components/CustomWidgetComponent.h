
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CustomWidgetComponent.generated.h"

/**
 * Дополненый WidgetComponent. Используйте этот виджет компонент, если вам надо получить ссылку на компонент из класса виджета. 
 * Класс виджета должен реализовывать интерфейс "IInitializableWidgetInterface".
 */
UCLASS(ClassGroup = "UserInterface", meta = (BlueprintSpawnableComponent, DisplayName = "Widget Component Legacy"))
class CALYPSO_DEV_API UCustomWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
};
