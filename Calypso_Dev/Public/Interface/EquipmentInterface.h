
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
	/** ���������� ����� ����� ��� ����������. */
	UFUNCTION(BlueprintNativeEvent, Category = "Equipment|Delegeta")
	void OnActorEquipment(AActor* ToActor, EEquipmentSlot ToSlot);

	/** ���������� ����� ����� ������ �� ����������. */
	UFUNCTION(BlueprintNativeEvent, Category = "Equipment|Delegeta")
	void OnActorUnEquipment(AActor* FromActor, EEquipmentSlot FromSlot);

	/** ��������� ��� ����� ����������, � ������� ����� ���� ������ �����. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "BPI Get Actor Equipment Slot"), Category = "Equipment|State")
	enum EEquipmentSlot GetActorEquipmentSlot();
};
