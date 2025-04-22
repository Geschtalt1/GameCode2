

#include "Classes/AttributeProxy.h"
#include "Components/AttributeComponent.h"

UAttributeProxy::UAttributeProxy(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAttributeProxy::Init(UAttributeComponent* InSource, UAttributeData* InData)
{
	if ((InSource != nullptr) && (InData != nullptr))
	{
		Data = InData;
		Source = InSource;
		
		OnInit();
	}
}

void UAttributeProxy::Destroyed()
{
	Data = nullptr;
	Source = nullptr;

	OnDestroed();
}

void UAttributeProxy::PreAttributeChange(const FGameplayTag& NameValue, float& NewValue)
{
	NewValue = PreAttributeChangeInternal(NameValue, NewValue);
}

float UAttributeProxy::PreAttributeChangeInternal_Implementation(const FGameplayTag& NameValue, float NewValue)
{
	return NewValue;
}

FAttributeInstance UAttributeProxy::GetOwnedAttribute() const
{
	if ((Source != nullptr) && (Data != nullptr))
	{
		return *Source->FindAttribute(Data->AttributeName);
	}
	return FAttributeInstance();
}

