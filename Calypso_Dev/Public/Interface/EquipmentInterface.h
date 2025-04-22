
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipmentInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CALYPSO_DEV_API IEquipmentInterface
{
	GENERATED_BODY()

public:
	/** Вызывается когда Актор был экипирован. */
	UFUNCTION(BlueprintNativeEvent, Category = "Equipment|Delegeta")
	void OnActorEquipment(AActor* ToActor, EEquipmentSlot ToSlot);

	/** Вызывается когда Актор больше не экипирован. */
	UFUNCTION(BlueprintNativeEvent, Category = "Equipment|Delegeta")
	void OnActorUnEquipment(AActor* FromActor, EEquipmentSlot FromSlot);

	/** Возращает тип слота экипировки, в который может быть пощене актор. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "BPI Get Actor Equipment Slot"), Category = "Equipment|State")
	enum EEquipmentSlot GetActorEquipmentSlot();
};
