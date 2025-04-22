
#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Classes/AttributeEffect.h"
#include "Data/AttributeData.h"
#include "GameplayTagContainer.h"

#include "AttributeComponent.generated.h"


/***/
USTRUCT(BlueprintType)
struct FAttributeInstance
{
    GENERATED_BODY()

public:
    FAttributeInstance()
    {}

    FAttributeInstance(UAttributeData* InData, UAttributeComponent* InSource)
        : Data(InData), 
        Source(InSource)
    {
        if (IsValid())
        {
            Values = Data->Values;
        }
    }

public:
    /** Возращает true, если аттрибут валидный. */
    FORCEINLINE bool IsValid() const { return Data != nullptr; }

    /** Возращает тег аттрибута. */
    FORCEINLINE FGameplayTag GetName() const { return Data->AttributeName; }

    /** Возращает true, если указаное значение существует. */
    FORCEINLINE bool HasValue(const FGameplayTag& NameValue) const { return Values.Contains(NameValue) != false; }

    /** Возращает базовое, значение аттрибута. */
   // FORCEINLINE float GetBaseValue() const { return (Proxy != nullptr) ? (Proxy->GetBaseValue()) : (-1.f); }

    /** Возращает текущее, значение аттрибута. */
   // FORCEINLINE float GetCurrentValue() const { return (Proxy != nullptr) ? (Proxy->GetCurrentValue()) : (-1.f); }

    /** Возращает всю TMap.*/
    FORCEINLINE const TMap<FGameplayTag, float>& GetValues() const { return Values; }

public:
    /** Добавляет attribute proxy class. */
    void AddProxy(class UAttributeProxy* InProxy);

    /** Удаляет текущий attribute proxy. */
    void RemoveProxy();

    /** Ищет значение в мапе стата. Возращает -1.f, если такой параметр не найден. */
    float FindValue(const FGameplayTag& NameValue);

    /***/
    void SetValue(const FGameplayTag& NameValue, float NewValue);

private:
    /** Значения аттрибута. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TMap<FGameplayTag, float> Values;

    /** Набор данных аттрибута. */
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAttributeData> Data;

    /** Аттрибут прокси. */
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UAttributeProxy> Proxy;

    /** Источник, где был создан аттрибут. */
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAttributeComponent> Source;
};

/** Структура предназначеная для инициализации значений аттрибута через Data Table. */
USTRUCT(BlueprintType)
struct FAttributeRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FORCEINLINE bool IsValid() const { return Values.Num() > 0; }

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FGameplayTag, float> Values;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorChangeAttribute, class AActor*, Actor, FAttributeInstance, ChangedAttribute, FGameplayTag, AttributeName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorAddAttribute, class AActor*, Actor, FAttributeInstance, AddedAttribute, FGameplayTag, AttributeName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorRemoveAttribute, class AActor*, Actor, FAttributeInstance, RemovedAttribute, FGameplayTag, AttributeName);

/**
* @brief Управляет набором атрибутов и предоставляет к нему доступ.
*
* `UAttributeComponent` — это компонент, который можно прикрепить к актеру для управления и
* предоставления доступа к набору атрибутов.
* для определения, хранения и изменения значений атрибутов, что позволяет использовать игровую механику, основанную на
* числовых характеристиках, таких как здоровье, мана, сила и т. д.
*
* Этот компонент обрабатывает создание, извлечение и изменение атрибутов, что делает его
* центральной точкой управления статистикой персонажа или объекта. Он также предоставляет методы для
* применения игровых эффектов, которые могут изменять значения атрибутов с течением времени или в ответ на
* определенные игровые события.
*
* Основные характеристики:
* - Хранит и управляет набором атрибутов.
* - Предоставляет методы для простого доступа и изменения значений атрибутов.
* - Поддерживает применение игровых эффектов для временных или постоянных изменений атрибутов.
* - Позволяет определять значения атрибутов по умолчанию и начальную настройку.
*
* Использование:
* - Добавьте этот компонент к Актеру, которому нужны атрибуты.
* - Определите атрибуты, которыми будет управлять этот компонент, создав пользовательский AttributeData.
* - Используйте предоставленные функции для получения, установки и изменения значений атрибутов.
* - Применяйте игровые эффекты к компоненту для временного или постоянного изменения значений атрибутов.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CALYPSO_DEV_API UAttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAttributeComponent(const FObjectInitializer& ObjectInitializer);

public:
    /** 
     * Добавляет новый атрибут в коллекцию атрибутов.
     *
     * @param AttributeData - Данные атрибута для добавления. Должны быть допустимым и уникальным атрибутом.
     * @return True, если атрибут был успешно добавлен, в противном случае false (например, если данные атрибута недопустимы или уже существуют).
    */
    UFUNCTION(BlueprintCallable, Category = "Utility|Add")
    bool AddAttribute(UAttributeData* AttributeData);

    /**
     * Удаляет атрибут из коллекции атрибутов.
     *
     * @param AttributeName Имя атрибута для удаления.
     * @return True, если атрибут был успешно удален, false в противном случае (например, если атрибут не существует).
    */
    UFUNCTION(BlueprintCallable, Category = "Utility|Remove")
    bool RemoveAttribute(FGameplayTag AttributeName);

    /**
     * @brief Извлекает значение определенного атрибута.
     *
     * Эта функция извлекает числовое значение, связанное с атрибутом, идентифицированным его `FGameplayTag`.
     * Она позволяет извлекать различные аспекты или свойства атрибута, указывая вторичный `FGameplayTag`
     * представляющий желаемое значение (например, базовое значение, текущее значение, максимальное значение). Если атрибут или
     * указанное имя значения не найдены, возвращается значение по умолчанию -1.0f.
     *
     * @param AttributeName `FGameplayTag`, который уникально идентифицирует атрибут, значение которого извлекается.
     * @param ValueName `FGameplayTag`, указывающий, какое значение атрибута извлекать
     * (например, "Attribute.Base", "Attribute.Current", "Attribute.Max").

     * @return Числовое значение (float) указанного атрибута. Возвращает -1.0f, если атрибут не найден или если указанное имя значения недопустимо для атрибута.
    */
    UFUNCTION(BlueprintCallable, Category = "Utility|Attributee")
    float GetAttributeValue(const FGameplayTag AttributeName, const FGameplayTag ValueName) const;

    /**
     * @brief Устанавливает значение определенного свойства атрибута.
     *
     * Эта функция изменяет значение, связанное с определенным свойством атрибута, идентифицируемым
     * комбинацией `AttributeName` и `ValueName`. Она позволяет применять различные типы
     * изменений, такие как переопределение текущего значения, добавление к нему или
     * умножение его. Если указанный атрибут не найден или `ValueName` недействителен, функция вернет `false` и никакие изменения не будут применены.
     * Триггерит событие OnActorChangedAttribute.
     *
     * @param AttributeName `FGameplayTag` - который уникально идентифицирует атрибут для изменения.
     * @param ValueName `FGameplayTag` - указывающий, какое свойство атрибута следует задать
     * (например, "Attribute.Base", "Attribute.Current", "Attribute.Multiplier").
     * @param NewValue Новое числовое значение для применения к свойству атрибута.
     * Интерпретация этого значения зависит от `ModifierType`.
     * @param ModifierType - Указывает, как `NewValue` следует применять к свойству
     * 
     * @return True, если атрибут найден и его значение успешно изменено. 
    */
    UFUNCTION(BlueprintCallable, Category = "Utility")
    bool SetAttributeValue(const FGameplayTag AttributeName, const FGameplayTag ValueName, float NewValue, EAttributeModifierType ModifierType = EAttributeModifierType::SM_Override);

    /**
     * @brief Находит атрибут в коллекции по его `FGameplayTag` и возвращает его.
     *
     * Эта функция ищет атрибут в управляемой коллекции атрибутов, используя
     * предоставленный `FGameplayTag` в качестве уникального идентификатора. Если найден атрибут с соответствующим
     * `AttributeName`, его информация копируется в предоставленный параметр `OutAttribute`.
     *
     * @param AttributeName - `FGameplayTag` для поиска. Этот тег уникально идентифицирует атрибут в коллекции.
     * @param OutAttribute - Ссылка на объект `FAttributeInstance`, который будет заполнен данными атрибута, если атрибут найден. 
     * Этот объект будет содержать все свойства, связанные с атрибутом.
     * 
     * @return True, если был найден атрибут с указанным `AttributeName` 
    */
    UFUNCTION(BlueprintCallable, Category = "Utility|Attributee")
    bool FindAttributeByName(FGameplayTag AttributeName, FAttributeInstance& OutAttribute) const;

    /**
     * @brief Проверяет, существует ли атрибут с указанным `FGameplayTag` в коллекции.
     *
     * Эта функция определяет, присутствует ли атрибут, идентифицированный его уникальным `FGameplayTag`,
     * в настоящее время в управляемой коллекции атрибутов. Она обеспечивает быстрый и
     * эффективный способ проверки существования атрибута перед попыткой доступа или
     * изменения его свойств.
     *
     * @param AttributeName `FGameplayTag` для поиска. Этот тег уникально идентифицирует атрибут в коллекции.
     *
     * @return True, если атрибут с указанным `AttributeName` существует в коллекции.
    */
    UFUNCTION(BlueprintCallable, Category = "Utility|Attributee")
    bool HasAttribute(const FGameplayTag AttributeName) const;

    /**
     * @brief Возвращает копию всех атрибутов и соответствующих им значений.
     * 
     * @return `TMap`, где ключом является `FGameplayTag`, представляющий
     * имя атрибута, а значением является объект `FAttributeInstance`, содержащий данные атрибута.
    */
    UFUNCTION(BlueprintCallable, Category = "Utility|Attributee")
    TMap<FGameplayTag, FAttributeInstance> GetAllAttributes() const;

public:
    /** Находит аттрибут в коллекции аттрибутов. */
    FORCEINLINE FAttributeInstance* FindAttribute(const FGameplayTag& AttributeName);

public:
    /** Применяет геймлей эффект к компоненту. */
    UFUNCTION(BlueprintCallable, Category = "Utility|Effects")
    bool ApplyAttributeEffectToSelf(TSubclassOf<UAttributeEffect> AttributeEffect);

    /***/
    UFUNCTION(BlueprintCallable, Category = "Utility|Effects")
    bool RemoveAttributeEffectToSelf(TSubclassOf<UAttributeEffect> AttributeEffect);

    /** Возращает true, если указаный эффект сейчас активный. */
    UFUNCTION(BlueprintCallable, Category = "Utility|Effects")
    bool HasActiveEffect(FGameplayTag AttributeEffectName);

public:
    /** Делегат добавления стата. */
    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Added Attribute"))
    FOnActorAddAttribute OnActorAddedAttribute;

    /** Делегат удаления стата. */
    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Removed Attribute"))
    FOnActorRemoveAttribute OnActorRemovedAttribute;

    /** Делегат обновления стата. */
    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Changed Attribute"))
    FOnActorChangeAttribute OnActorChangedAttribute;

protected:
    virtual void BeginPlay() override;
    virtual void PostLoad() override;
    virtual void PostInitProperties() override;

private:
    /** Функция для инициализации статов на основе списка UAttributeData в массиве DefaultAttributes. Можно заполнять статы из Data Table. */
    void InitializeAttributes();

    /** Функция для инициализации начальных эффектов. */
    void InitializeEffects();

    /** Находит начальное значение в DataTableValues. */
    FAttributeRow FindInitValue(const FGameplayTag& AttributeName) const;

    /** Рассчитывает новое значение. */
    void CalculateNewValue(float& AttributeValue, const float& NewValue, const EAttributeModifierType& ModifierType);

    /** Создает аттрибут. */
    FAttributeInstance MakeAttribute(UAttributeData* AttributeData);

private:
    /** Список начальных статов. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Attributes|Init")
    TArray<TObjectPtr<class UAttributeData>> DefaultAttributes;

    /** Список начальных эффектов. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Attributes|Init")
    TArray<TSubclassOf<UAttributeEffect>> StartupEffects;

    /** Таблица начальных значений статов. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Attributes|Init")
    TObjectPtr<UDataTable> DataTableValues;

private:
    /*
     * Карта, содержащая все статы владельца компонента, где FGameplayTag - имя стата,
     * FAttributeInstance - структура, содержащая текущее значение и другие параметры.
    */
    UPROPERTY()
    TMap<FGameplayTag, FAttributeInstance> Attributes;

     /*
      * Карта содержащия все эффекты статов, где FGameplayTag - имя эффекта,
      * FAttributeInstanceEffectSpec - структура, содержащая данные эффекта.
      */
     UPROPERTY()
     TMap<FGameplayTag, UAttributeEffect*> Effects;
};
