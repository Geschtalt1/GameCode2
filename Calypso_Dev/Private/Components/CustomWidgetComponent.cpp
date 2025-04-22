

#include "Components/CustomWidgetComponent.h"

#include "Interface/InitializableWidgetInterface.h"

void UCustomWidgetComponent::InitWidget()
{
	Super::InitWidget();

	if (GetUserWidgetObject() != nullptr)
	{
		// Check has init widget interface.
		if (GetUserWidgetObject()->Implements<UInitializableWidgetInterface>())
		{
			// Call interface in the widget object.
			IInitializableWidgetInterface::Execute_OnCreatedWidget(GetUserWidgetObject(), this);
		}
	}
}