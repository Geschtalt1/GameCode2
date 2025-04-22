

#include "Components/EquipmentComponent.h"

#include "Interface/EquipmentInterface.h"

UEquipmentComponent::UEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

}


void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UEquipmentComponent::TryAddEquipmentToSlot(AActor* NewEquip, EEquipmentSlot ToSlot)
{
	if (CheckActorEquipment(NewEquip) != false)
	{
		RemoveEquipment(ToSlot); // Try remove current slot.

		// Set owner & call interface.
		NewEquip->SetOwner(GetOwner());
		IEquipmentInterface::Execute_OnActorEquipment(NewEquip, GetOwner(), ToSlot);

		// Equip add in map.
		Equipments.Add(ToSlot, NewEquip);

		// Call delegate.
		OnActorAddedEquipment.Broadcast(GetOwner(), NewEquip, ToSlot);
		return true;
	}
	return false;
}

bool UEquipmentComponent::CommitEquipment(AActor* NewEquip)
{
	if (CheckActorEquipment(NewEquip) != false)
	{
		const EEquipmentSlot SlotLocal = IEquipmentInterface::Execute_GetActorEquipmentSlot(NewEquip);
		return TryAddEquipmentToSlot(NewEquip, SlotLocal);
	}
	return false;
}

bool UEquipmentComponent::FindEquipmentBySlot(EEquipmentSlot InSlot, AActor*& OutEquipment) const
{
	if (Equipments.Contains(InSlot) != false)
	{
		OutEquipment = *Equipments.Find(InSlot);
		return OutEquipment != nullptr;
	}
	return false;
}

bool UEquipmentComponent::RemoveEquipment(EEquipmentSlot FromSlot, bool bDestroy)
{
	AActor* RemoveEquip;
	if (FindEquipmentBySlot(FromSlot, RemoveEquip) != false)
	{
		// Remove owner.
		RemoveEquip->SetOwner(nullptr);

		// Call interface in remove equipment.
		IEquipmentInterface::Execute_OnActorUnEquipment(RemoveEquip, GetOwner(), FromSlot);

		// Remove slot from MAP.
		Equipments.Remove(FromSlot);

		// Call delegate remove equipment.
		OnActorRemovedEquipment.Broadcast(GetOwner(), RemoveEquip, FromSlot);
		
		if (bDestroy != false) { RemoveEquip->Destroy(); }
		return true;
	}
	return false;
}

void UEquipmentComponent::ClearEquipments(bool bDestroy)
{
	for (const auto& Pair : Equipments)
	{
		RemoveEquipment(Pair.Key, bDestroy);
	}
}

bool UEquipmentComponent::CheckActorEquipment(AActor* CheckActor) const
{
	if (CheckActor != nullptr)
	{
		return CheckActor->Implements<UEquipmentInterface>();
	}
	return false;
}

bool UEquipmentComponent::UnArmed()
{
	AActor* UsableActor;
	if (FindEquipmentBySlot(EEquipmentSlot::ES_Usable, UsableActor) != false)
	{
		RemoveEquipment(EEquipmentSlot::ES_Usable);
		return CommitEquipment(UsableActor);
	}
	return false;
}