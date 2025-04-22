
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	/** Слот позволят использовать предмет, находящийся в руке. */
	ES_Usable        UMETA(DisplayName = "Usable"),

	/** Кобура для пистолета. */
	ES_Pistol        UMETA(DisplayName = "Pistol Holster"),

	/** Кобура для винтовки. */
	ES_Rifle         UMETA(DisplayName = "Rifle Holster"),

	/** Слот для джетпака. */
	ES_Jetpack       UMETA(DisplayName = "Jetpack")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorAddedEquipment, class AActor*, Actor, class AActor*, AddedEquipment, EEquipmentSlot, InSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorRemovedEquipment, class AActor*, Actor, class AActor*, RemovedEquipment, EEquipmentSlot, FromSlot);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CALYPSO_DEV_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent(const FObjectInitializer& ObjectInitializer);

public:
	/** Иницилизирует экипировку в слот персонажа. */
	UFUNCTION(BlueprintCallable, Category = "Utility|Add")
	bool CommitEquipment(AActor* NewEquip);

	/** Принудительно пробует добавить экипировку в указанный слот. */
	UFUNCTION(BlueprintCallable, Category = "Utility|Add")
	bool TryAddEquipmentToSlot(AActor* NewEquip, EEquipmentSlot ToSlot);

	/** Находит экипировку в слоте и возращает ее. */
	UFUNCTION(BlueprintCallable, Category = "Utility|State")
	bool FindEquipmentBySlot(EEquipmentSlot InSlot, AActor*& OutEquipment) const;

	/** Возращает все текущие слоты экипировки. */
	UFUNCTION(BlueprintCallable, Category = "Utility|State")
	FORCEINLINE TMap<EEquipmentSlot, AActor*> GetAllEquipments() const { return Equipments; }

	/** Обежаруживает владельца, убирая предмет из слота Usable, в слот предмета. */
	UFUNCTION(BlueprintCallable, Category = "Utility|State")
	bool UnArmed();

	/** Удаляет экипировку из слота. */
	UFUNCTION(BlueprintCallable, Category = "Utility|Remove")
	bool RemoveEquipment(EEquipmentSlot FromSlot, bool bDestroy = false);

	/** Удаляет всю экипировку владельца. */
	UFUNCTION(BlueprintCallable, Category = "Utility|Remove")
	void ClearEquipments(bool bDestroy = false);

public:
	/** Делегат добавление новой экипировки. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Added Equipment"))
	FOnActorAddedEquipment OnActorAddedEquipment;

	/** Делегат удаление экипировки. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Removed Equipment"))
	FOnActorRemovedEquipment OnActorRemovedEquipment;

protected:
	virtual void BeginPlay() override;

private:
	/** Проверяет актор на наличие интерфейса "UEquipmentInterface". */
	FORCEINLINE bool CheckActorEquipment(AActor* CheckActor) const;

private:
	/** Экипировка владельца. */
	UPROPERTY()
	TMap<EEquipmentSlot, AActor*> Equipments;
};
