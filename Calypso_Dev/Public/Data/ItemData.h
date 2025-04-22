
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_Equipment        UMETA(DisplayName = "Equipment"),
	IT_Resources        UMETA(DisplayName = "Resources"),
	IT_Consumable       UMETA(DisplayName = "Consumable")
};

/**
 * Класс ItemData предназначен для хранения информации об игровых предметах (items).
 * Вы можете расширить его, добавив новые свойства или функции.
 */
UCLASS()
class CALYPSO_DEV_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** Unique item name. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Setting")
	FName ItemName;

	/** Display name. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Setting")
	FText ItemDisplayName;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Setting")
	FText ItemDescription;

	/** Icon. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Setting")
	TSoftObjectPtr<UTexture2D> ItemIcon = nullptr;

	/** Actor in world. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Setting")
	TSoftClassPtr<AActor> ItemActorClassAsset = nullptr;

	/** Type item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Setting")
	EItemType ItemType = EItemType::IT_Consumable;

	/** Maximum stack. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Setting")
	int32 ItemMaxStack = 1;
};
