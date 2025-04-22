
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "AttributeProxy.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, meta = (ShowWorldContextPin))
class CALYPSO_DEV_API UAttributeProxy : public UObject
{
	GENERATED_BODY()
	
public:
	UAttributeProxy(const FObjectInitializer& ObjectInitializer);

public:
	/** 
	 * @brief Инициализирует `UAttributeProxy` с указанным компонентом-владельцем и данными атрибута.
	 *
	 * Эта функция устанавливает внутреннее состояние `UAttributeProxy`, связывая его
	 * с определенным `UAttributeComponent` и ресурсом `UAttributeData`. `UAttributeComponent`
	 * становится источником значения атрибута, в то время как ресурс `UAttributeData` предоставляет
	 * конфигурацию атрибута, такую ​​как его имя, значение по умолчанию и любые другие метаданные.
	*/
	virtual void Init(class UAttributeComponent* InSource, class UAttributeData* InData);

	/**
	 * @brief Вызывается, когда Актер уничтожается и до того, как будут отвязаны все компоненты.
	 *
	 * Эта функция является собственным событием C++, которое вызывается для Актера, когда он собирается
	 * быть уничтоженным. Это последняя возможность выполнить любую логику очистки или завершения
	 * перед тем, как Актер будет полностью удален из мира и его память будет освобождена.
	*/
	virtual void Destroyed();

	/**
	* @brief Вызывается перед тем, как значение атрибута должно измениться, предоставляя возможность изменить новое значение или предотвратить изменение.
	*
	* Эта функция позволяет перехватывать изменения атрибутов и выполнять пользовательскую логику
	* до того, как значение атрибута будет фактически обновлено. Эту функцию можно использовать для:
	* - проверки предлагаемого нового значения.
	* - фиксации нового значения в определенном диапазоне.
	* - применения пользовательского масштабирования или преобразований к новому значению.
	* - предотвращения изменения атрибута в целом.
	*
	* @param NameValue `FGameplayTag`, идентифицирующее значение, которое должно измениться.
	* @param NewValue Ссылка на предлагаемое новое значение для атрибута.
	*/
	void PreAttributeChange(const FGameplayTag& NameValue, float& NewValue);

public:
	/**
	 * @brief Возвращает указатель на актив `UAttributeData`, принадлежащий этому объекту.
	 *
	 * Эта функция обеспечивает прямой доступ к активу `UAttributeData`, который определяет
	 * свойства этого атрибута. Возвращенный указатель позволяет вам проверить
	 * конфигурацию атрибута, такую ​​как его имя, значение по умолчанию, минимальные/максимальные значения,
	 * и любые другие метаданные, хранящиеся в активе `UAttributeData`.
	 *
	 * @return Указатель на актив `UAttributeData`, принадлежащий этому объекту. Возвращает `nullptr`, если с этим объектом не связан актив `UAttributeData`.
	 */
	UFUNCTION(BlueprintCallable, Category = "Attribute Proxy")
	FORCEINLINE class UAttributeData* GetOwnedData() const { return Data; };

	/**
	 * @brief Возвращает указатель на `UAttributeComponent`, который владеет и управляет этим прокси атрибутом.
	 *
	 * Эта функция предоставляет доступ к `UAttributeComponent`, который отвечает за
	 * хранение и управление этим атрибутом. Возвращенный указатель позволяет взаимодействовать с
	 * владеющим компонентом, например, для доступа к другим атрибутам в той же коллекции,
	 * применять игровые эффекты или извлекать информацию о владеющем актере.
	 *
	 * @return Указатель на `UAttributeComponent`, который владеет этим атрибутом. Возвращает `nullptr`, если этот атрибут не принадлежит ни одному `UAttributeComponent`.
	*/
	UFUNCTION(BlueprintCallable, Category = "Attribute Proxy")
	FORCEINLINE class UAttributeComponent* GetOwnedSource() const { return Source; }

	/**
	* @brief Возвращает копию структуры `FAttributeInstance`, представляющую текущее состояние атрибута.
	*
	* Эта функция предоставляет снимок данных атрибута, включая его значение,
	* модификаторы и другую соответствующую информацию, инкапсулированную в структуру `FAttributeInstance`. 
	* Это позволяет вам проверять состояние атрибута без прямогодоступа к базовому хранилищу атрибутов.
	*
	* @return Копия структуры `FAttributeInstance`. Изменение этой копии не повлияет на исходный атрибут. 
	*/
	UFUNCTION(BlueprintCallable, Category = "Attribute Proxy")
	FORCEINLINE struct FAttributeInstance GetOwnedAttribute() const;

protected:
	/**
	 * @brief Вызывается после инициализации Attribute Proxy и готовности к использованию.
	 *
	 * Это событие предоставляет выделенное место для Blueprints для реализации пользовательской логики инициализации для Attribute Proxy.
	 * Он гарантированно вызывается после выполнения внутренней функции `Init`, гарантируя, что элементы `Source` и `Data` действительны и доступны.
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Init"), Category = "Attribute Proxy")
	void OnInit();

	/**
	 * @brief Вызывается, когда `UAttributeProxy` собирается быть уничтожен.
	 *
	 * Это событие предоставляет выделенное место для Blueprints для реализации пользовательской логики очистки для Attribute Proxy.
	 * Он гарантированно вызывается до полного уничтожения объекта, позволяя вам освободить любые ресурсы или выполнить любые другие необходимые операции очистки.
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Destroed"), Category = "Attribute Proxy")
	void OnDestroed();

	/** 
	 * @brief Вызывается перед тем, как значение атрибута должно измениться, предоставляя возможность изменить новое значение или предотвратить изменение.
	 * 
	 * Это внутренняя реализация, предназначенная для настройки как C++, так и Blueprint. 
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Pre Attribute Change"), Category = "Attribute Proxy")
	float PreAttributeChangeInternal(const FGameplayTag& NameValue, float NewValue);
	virtual float PreAttributeChangeInternal_Implementation(const FGameplayTag& NameValue, float NewValue);

private:
	/** @brief Указатель на ресурс `UAttributeData`, который определяет свойства атрибута.*/
	UPROPERTY()
	TObjectPtr<class UAttributeData> Data;

	/** @brief Указатель на источник где был создан и хранится аттрибут. */
	UPROPERTY()
	TObjectPtr<class UAttributeComponent> Source;
};
