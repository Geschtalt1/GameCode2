

#include "Classes/AttributeEffect.h"

#include "Components/AttributeComponent.h"

UAttributeEffect::UAttributeEffect()
{
}


bool UAttributeEffect::OnApplied(UAttributeComponent* InSource)
{
	if (InSource != nullptr)
	{
		Source = InSource;

		switch (DurationPolicy)
		{

		case EDurationPolicy::DP_Instant:
		{
			// Применяет подификаторы моментально.
			ApplyModifiers();

		} break;

		case EDurationPolicy::DP_Duration:
		{
			// Start timer.
			SetPeriodic(Period);

		} break;

		case EDurationPolicy::DP_Infinite:
		{
			// Start timer.
			SetPeriodic(Period);

		} break;
		}

		return true;
	}
	return false;
}

bool UAttributeEffect::OnRemoved(UAttributeComponent* InSource)
{
	if (InSource != nullptr)
	{

	}
	return false;
}

void UAttributeEffect::SetPeriodic(float NewPeriodic)
{
	if (NewPeriodic > 0.0f)
	{
		TimeElapsed = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(PeriodicTimerHandle);

		// Start period effect tick.
		GetWorld()->GetTimerManager().SetTimer(
			PeriodicTimerHandle,
			this,
			&UAttributeEffect::OnPeriodicEffectTick,
			NewPeriodic,
			true,
			(bExecutePeriodEffectOnApplication != false) ? (NewPeriodic) : (0.0f)
		);
	}
}

void UAttributeEffect::OnPeriodicEffectTick()
{
	// Check if selected duration.
	if (DurationPolicy == EDurationPolicy::DP_Duration)
	{
		TimeElapsed += Period;
		if (TimeElapsed >= Duration)
		{
			GetWorld()->GetTimerManager().ClearTimer(PeriodicTimerHandle);
		}
	}

	ApplyModifiers();
}

void UAttributeEffect::ApplyModifiers()
{
	if ((Source != nullptr) && (Modifiers.Num() > 0))
	{
		// Применяем модификаторы к статам.
		for (auto& ModifireLocal : Modifiers)
		{
			/*AttributeComponent->SetAttributeValue(
				ModifireLocal.AttributeName,
				ModifireLocal.ModifierValue,
				ModifireLocal.ModifierType
			);*/
		}
	}
}