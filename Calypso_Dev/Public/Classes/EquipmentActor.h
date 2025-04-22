
#pragma once

#include "CoreMinimal.h"
#include "Classes/ItemBase.h"
#include "Interface/EquipmentInterface.h"
#include "EquipmentActor.generated.h"

UCLASS(Abstract, meta = (DisplayName = "Equipment Base"))
class CALYPSO_DEV_API AEquipmentActor : public AItemBase, public IEquipmentInterface
{
	GENERATED_BODY()
	
public:	
	AEquipmentActor(const FObjectInitializer& ObjectInitializer);

public:
	/** Пробует прикрепить Актор, к владельцу. */
	UFUNCTION(BlueprintCallable, Category = "Utility|Attach")
	bool TryAttachToComponent(FName TagComponent, FName SocketName = FName(""), FTransform Offset = FTransform());

public:
	virtual void OnActorEquipment_Implementation(AActor* ToActor, EEquipmentSlot ToSlot) override;
	virtual void OnActorUnEquipment_Implementation(AActor* FromActor, EEquipmentSlot FromSlot) override;
	virtual EEquipmentSlot GetActorEquipmentSlot_Implementation() override;

protected:
	/**
	 * Custom Event - Specify the owner component tag to which the Actor will be attached, the socket tag, and the relative offset.
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Begin Attachment To Component"), Category = Equipment)
	void OnBeginAttachmentToComponent(EEquipmentSlot ToSlot, FName& OutTagComponent, FName& OutSocketName, FTransform& OutOffset);
	virtual void OnBeginAttachmentToComponent_Implementation(EEquipmentSlot ToSlot, FName& OutTagComponent, FName& OutSocketName, FTransform& OutOffset);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
