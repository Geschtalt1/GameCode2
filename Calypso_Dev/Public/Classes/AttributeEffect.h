
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "AttributeEffect.generated.h"

/** Тип эффекта. */
UENUM(BlueprintType)
enum class EDurationPolicy : uint8
{
	// Мгновеный.
	DP_Instant           UMETA(DisplayName = "Instant"),

	// Временный.
	DP_Duration          UMETA(DisplayName = "Has Duration"),

	// Бесконечный.
	DP_Infinite          UMETA(DisplayName = "Infinite")
};

/** Перечисление, определяющее тип модификатора. */
UENUM(BlueprintType)
enum class EAttributeModifierType : uint8
{
	// Прибавляет значение к текущему значению стата.
	SM_Additive           UMETA(DisplayName = "Additive"),

	// Умножает текущее значение стата на значение модификатора
	SM_Multiplicative     UMETA(DisplayName = "Multiply"),

	// Заменяет текущее значение стата на значение модификатора.
	SM_Override           UMETA(DisplayName = "Override"),

	MAX UMETA(Hidden)
};

/** Базовая структура для определения модификаторов статов. */
USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()

public:
	/** Имя стата, к которому применяется модификатор. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeName;

	/** Тип модификатора. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAttributeModifierType ModifierType;

	/** Значение модификатора. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ModifierValue;
};

/**
* Представляет собой временный эффект, влияющий на статы.
* Может использоваться для баффов, дебаффов, и т.д.
*/
UCLASS(Abstract, Blueprintable, meta = (ShowWorldContextPin))
class CALYPSO_DEV_API UAttributeEffect : public UObject
{
	GENERATED_BODY()

public:
	UAttributeEffect();

public:
	/** Применяет все модификаторы к целевому компоненту. */
	bool OnApplied(class UAttributeComponent* InSource);

	/** Отменяет действие всех модификаторов. */
	bool OnRemoved(class UAttributeComponent* InSource);

public:
	/** Тэг модификатора. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute Effect")
	FGameplayTag AttributeEffectName;

	/*
	 * Тип эффекта: Instant, Duration или Infinite.
	 * Для Duration и Infinite доступна настройка периодичности.
	 * Если указан период, то эффект будет срабатывать как Instant раз в период на протяжении времени действия.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute Effect|Duration")
	EDurationPolicy DurationPolicy = EDurationPolicy::DP_Instant;

	/** Период применения эффекта. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "DurationPolicy != EDurationPolicy::DP_Instant", EditConditionHides), Category = "Attribute Effect|Duration")
	float Period;

	/** Продолжительность эффекта. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "DurationPolicy == EDurationPolicy::DP_Duration", EditConditionHides), Category = "Attribute Effect|Duration")
	float Duration;

	/** Единичные изменения стата. Для каждого Modifier’а нужно указать стат, способ его изменения (добавление, умножение, замена значения), величину изменения. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute Effect|Gameplay")
	TArray<FAttributeModifier> Modifiers;

protected:
	/** Определяет, будет ли немедленно выполнен периодический эффект, при применении этого Attribute Effect. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "DurationPolicy != EDurationPolicy::DP_Instant", EditConditionHides), Category = "Attribute Effect|Duration")
	bool bExecutePeriodEffectOnApplication;

protected:
	/** UTILITY */

	/** Запускает OnPeriodicEffectTick().  */
	void SetPeriodic(float NewPeriodic);

protected:
	/***/
	virtual void OnPeriodicEffectTick();

	/** Возращает указатель на мир. */
	FORCEINLINE UWorld* GetWorld() const { return GEngine->GetWorld(); }

private:
	/** Применяет модификаторы. */
	void ApplyModifiers();

private:
	/***/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Attribute Effect")
	TObjectPtr<class UAttributeComponent> Source;

	/** Сохраняем время последнего тика, чтоб высчитывать дельту. */
	FTimerHandle PeriodicTimerHandle;
	float TimeElapsed;
};
