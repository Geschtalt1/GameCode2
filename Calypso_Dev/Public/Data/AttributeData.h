
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeData.generated.h"

/**
* @brief Ресурс данных, определяющий свойства одного атрибута.
*
* Класс `UAttributeData` — это ресурс данных, разработанный для инкапсуляции определения одного
* атрибута в игре. Он хранит такую ​​информацию, как имя атрибута, значение по умолчанию,
* и любые другие свойства, относящиеся к поведению атрибута. Этот класс служит центральным
* репозиторием метаданных атрибутов, позволяя легко создавать, изменять и повторно использовать
* определения атрибутов в различных игровых объектах и ​​системах.
*
* Основные характеристики:
* - Сохраняет имя атрибута, обычно представленное как `FGameplayTag`.
* - Определяет значение атрибута по умолчанию.
* - Предоставляет способ указать минимальное и максимальное допустимые значения для атрибута.
* - Может быть расширен для включения дополнительных свойств, относящихся к определенным типам атрибутов
* (например, скорость регенерации, тип сопротивления).
*/
UCLASS()
class CALYPSO_DEV_API UAttributeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** Уникальное имя аттрибута. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Setting")
	FGameplayTag AttributeName;

	/** Отображаемое имя аттрибута для UI. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Setting")
	FText DisplayName;

	/** Описание аттрибута. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Setting")
	FText Description;

	/** Иконка аттрибута для UI. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Setting")
	TSoftObjectPtr<UTexture2D> Icon;

	/** Начальные значения аттрибута (например, "текущее", "базовое" и тп). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Setting")
	TMap<FGameplayTag, float> Values;

	/** Теги, определяющие, к каким объектам относится этот аттрибут (например, "Character", "Weapon", "Armor"). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Setting")
	FGameplayTagContainer RelevantTags;

	/** Класс, используемый для создания модификаторов аттрибута (опционально, для кастомной логики). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Setting")
	TSubclassOf<class UAttributeProxy> AttributeProxyClass;
};
