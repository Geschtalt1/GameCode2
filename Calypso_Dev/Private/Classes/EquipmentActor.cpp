

#include "Classes/EquipmentActor.h"

AEquipmentActor::AEquipmentActor(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEquipmentActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AEquipmentActor::TryAttachToComponent(FName TagComponent, FName SocketName, FTransform Offset)
{
	if ((GetOwner() != nullptr))
	{
		if (auto FoundComponent = GetOwner()->FindComponentByTag<USceneComponent>(TagComponent))
		{
			const FAttachmentTransformRules AttachmentTransformRules = { EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true };
			AttachToComponent(FoundComponent, AttachmentTransformRules, SocketName);
			SetActorRelativeTransform(Offset);
			
			return true;
		}
	}
	return false;
}

void AEquipmentActor::OnBeginAttachmentToComponent_Implementation(EEquipmentSlot ToSlot, FName& OutTagComponent, FName& OutSocketName, FTransform& OutOffset)
{
}

void AEquipmentActor::OnActorEquipment_Implementation(AActor* ToActor, EEquipmentSlot ToSlot)
{
	FTransform InOffset;
	FName InTagComponent, InSocketName;
	OnBeginAttachmentToComponent(ToSlot, InTagComponent, InSocketName, InOffset);

	// Try attach to owner component.
	TryAttachToComponent(InTagComponent, InSocketName, InOffset);
}

void AEquipmentActor::OnActorUnEquipment_Implementation(AActor* FromActor, EEquipmentSlot FromSlot)
{
}

EEquipmentSlot AEquipmentActor::GetActorEquipmentSlot_Implementation()
{
	return EEquipmentSlot();
}