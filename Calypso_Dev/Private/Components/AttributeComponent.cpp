

#include "Components/AttributeComponent.h"
#include "Data/AttributeData.h"

#include "Classes/AttributeEffect.h"
#include "Classes/AttributeProxy.h"

#include "BlueprintGameplayTagLibrary.h"

#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"

UAttributeComponent::UAttributeComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributeComponent::BeginPlay()
{
	InitializeAttributes();
	InitializeEffects();

	Super::BeginPlay();
}

void UAttributeComponent::PostLoad()
{
	Super::PostLoad();
}

void UAttributeComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

void UAttributeComponent::InitializeAttributes()
{
	Attributes.Empty();

	if (DefaultAttributes.IsEmpty())
	{
		return;
	}

	// Fill init Attributes.
	for (auto& DataLocal : DefaultAttributes)
	{
		// Добавляем аттрибут в коллекцию
		if (AddAttribute(DataLocal) != false)
		{
			// Заполняем аттрибуты начальными данными, если валидна Data Table.
			if (DataTableValues != nullptr)
			{
				FAttributeInstance* AddedAttribute = FindAttribute(DataLocal->AttributeName);

				// Находим начальные значения в таблице.
				FAttributeRow AttributeLocal = FindInitValue(DataLocal->AttributeName);
				
				// Устанавливаем начальные значения аттрибута.
				for (auto& Pair : AttributeLocal.Values)
				{
					AddedAttribute->SetValue(Pair.Key, Pair.Value);
				}
			}
		}
	}
}

void UAttributeComponent::InitializeEffects()
{
	if (StartupEffects.Num() > 0)
	{
		// Fill startup effects.
		for (auto& EffectLocal : StartupEffects)
		{
			ApplyAttributeEffectToSelf(EffectLocal);
		}
	}
}

FAttributeRow UAttributeComponent::FindInitValue(const FGameplayTag& AttributeName) const
{
	if ((DataTableValues != nullptr) && (AttributeName.IsValid() != false))
	{
		// Get Tag Name;
		FName TagName = UBlueprintGameplayTagLibrary::GetTagName(AttributeName);

		// Get Last Tag Name.
		FString LeftS, RightS;
		UKismetStringLibrary::Split(TagName.ToString(), FString("."), LeftS, RightS, ESearchCase::IgnoreCase, ESearchDir::FromStart);

		// Get Value From Data Table.
		FAttributeRow* AttributeLocal = DataTableValues->FindRow<FAttributeRow>(UKismetStringLibrary::Conv_StringToName(RightS), FString());

		return AttributeLocal != nullptr ? *AttributeLocal : FAttributeRow();
	}
	return FAttributeRow();
}

bool UAttributeComponent::AddAttribute(UAttributeData* AttributeData)
{
	// Check a Attribute data IsValid.
	if ((AttributeData != nullptr) && (HasAttribute(AttributeData->AttributeName) != true))
	{
		// Create new attribute.
		FAttributeInstance NewAttribute = MakeAttribute(AttributeData);
		if (NewAttribute.IsValid() != false)
		{
			// Add to TMap
			Attributes.Add(
				NewAttribute.GetName(),
				NewAttribute
			);

			return true;
		}
	}
	return false;
}

bool UAttributeComponent::RemoveAttribute(FGameplayTag AttributeName)
{
	if (HasAttribute(AttributeName) != false)
	{
		// Cash Attribute.
		FAttributeInstance* RemovedAttribute = Attributes.Find(AttributeName);
		RemovedAttribute->RemoveProxy();

		Attributes.Remove(AttributeName);
		OnActorRemovedAttribute.Broadcast(GetOwner(), *RemovedAttribute, AttributeName);
		return true;
	}
	return false;
}

float UAttributeComponent::GetAttributeValue(const FGameplayTag AttributeName, const FGameplayTag ValueName) const
{
	FAttributeInstance AttributeLocal;
	if (FindAttributeByName(AttributeName, AttributeLocal) != false)
	{
		return AttributeLocal.FindValue(ValueName);
	}
	return -1.0f;
}

FAttributeInstance* UAttributeComponent::FindAttribute(const FGameplayTag& AttributeName)
{
	if (Attributes.Contains(AttributeName))
	{
		return Attributes.Find(AttributeName);
	}
	return nullptr;
}

bool UAttributeComponent::SetAttributeValue(const FGameplayTag AttributeName, const FGameplayTag ValueName, float NewValue, EAttributeModifierType ModifierType)
{
	if (FAttributeInstance* AttributeLoc = FindAttribute(AttributeName))
	{
		float AttributeValue = AttributeLoc->FindValue(ValueName);
		if (AttributeValue != -1.0f)
		{
			// Рассчитываем новое значения для аттрибута с учетом модификации.
			CalculateNewValue(AttributeValue, NewValue, ModifierType);

			// Задаем новое значение в аттрибут.
			AttributeLoc->SetValue(ValueName, AttributeValue);

			OnActorChangedAttribute.Broadcast(GetOwner(), *AttributeLoc, AttributeName);
			return true;
		}
	}
	return false;
}

void UAttributeComponent::CalculateNewValue(float& AttributeValue, const float& NewValue, const EAttributeModifierType& ModifierType)
{
	switch (ModifierType)
	{
		// Additive value.
	case EAttributeModifierType::SM_Additive:
	{
		AttributeValue += NewValue;
	} break;

		// Multiplay value.
	case EAttributeModifierType::SM_Multiplicative:
	{
		AttributeValue *= NewValue;
	} break;

		// Override value;
	case EAttributeModifierType::SM_Override:
	{
		AttributeValue = NewValue;
	} break;
	}

}

bool UAttributeComponent::FindAttributeByName(FGameplayTag AttributeName, FAttributeInstance& OutAttribute) const
{
	if (HasAttribute(AttributeName) != false)
	{
		OutAttribute = *Attributes.Find(AttributeName);
		return true;
	}
	return false;
}

bool UAttributeComponent::HasAttribute(const FGameplayTag AttributeName) const
{
	return Attributes.Contains(AttributeName) != false;
}

TMap<FGameplayTag, FAttributeInstance> UAttributeComponent::GetAllAttributes() const
{
	return Attributes;
}

bool UAttributeComponent::ApplyAttributeEffectToSelf(TSubclassOf<UAttributeEffect> AttributeEffect)
{
	if ((AttributeEffect != nullptr))
	{
		// Проверяем что такого эффекта не добавлено.
		if (HasActiveEffect(AttributeEffect.GetDefaultObject()->AttributeEffectName) != false)
		{
			return false;
		}

		// Spawn Attribute effect.
		/*auto AttributeModEffect = GetWorld()->SpawnActorDeferred<UAttributeEffect>(
			AttributeEffect,
			GetOwner()->GetActorTransform(),
			GetOwner()
		);*/

		// Finish spawn.
		//UGameplayStatics::FinishSpawningActor(AttributeModEffect, GetOwner()->GetActorTransform());

		//if (AttributeModEffect->DurationPolicy != EDurationPolicy::DP_Instant)
		//{
		//}

		// Try apply effect.
		//return AttributeModEffect->OnApplied(this);
	}
	return false;
}

bool UAttributeComponent::RemoveAttributeEffectToSelf(TSubclassOf<UAttributeEffect> AttributeEffect)
{
	return false;
}

bool UAttributeComponent::HasActiveEffect(FGameplayTag AttributeEffectName)
{
	if (AttributeEffectName.IsValid() != false)
	{
		return Effects.Contains(AttributeEffectName);
	}
	return false;
}

FAttributeInstance UAttributeComponent::MakeAttribute(UAttributeData* AttributeData)
{
	if ((AttributeData != nullptr) && (AttributeData->Values.Num() > 0))
	{
		// Создаем новый аттрибут на основе данных.
		FAttributeInstance NewAttribute = FAttributeInstance(AttributeData, this);

		if (AttributeData->AttributeProxyClass != nullptr)
		{
			// Создаем прокси объект аттрибута.
			UAttributeProxy* AttributeProxy = NewObject<UAttributeProxy>(
				this,
				AttributeData->AttributeProxyClass
			);

			// Помещаем прокси в аттрибут.
			NewAttribute.AddProxy(AttributeProxy);
		}
		return NewAttribute;
	}
	return FAttributeInstance();
}

void FAttributeInstance::AddProxy(UAttributeProxy* InProxy)
{
	if ((InProxy != nullptr) && (Proxy == nullptr))
	{
		Proxy = InProxy;
		Proxy->Init(Source, Data);
	}
}

void FAttributeInstance::RemoveProxy()
{
	if (Proxy != nullptr)
	{
		Proxy->Destroyed();
		Proxy = nullptr;
	}
}

float FAttributeInstance::FindValue(const FGameplayTag& NameValue)
{
	if (Values.Contains(NameValue) != false)
	{
		return *Values.Find(NameValue);
	}
	return -1.f;
}

void FAttributeInstance::SetValue(const FGameplayTag& NameValue, float NewValue)
{
	if (HasValue(NameValue) != false)
	{
		if (Proxy != nullptr)
		{
			// Пробуем перехватить изменение значения в прокси.
			Proxy->PreAttributeChange(NameValue, NewValue);
		}

		// Меняем значение.
		float* ValuePtr = Values.Find(NameValue);
		*ValuePtr = NewValue;
	}
}
