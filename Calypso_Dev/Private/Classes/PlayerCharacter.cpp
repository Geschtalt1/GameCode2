

#include "Classes/PlayerCharacter.h"

#include "Components/CharacterSlidingComponent.h"
#include "Components/GameplayTagComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/PushingComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/AttributeComponent.h"


APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Sliding Component.
	CharacterSliding = CreateDefaultSubobject<UCharacterSlidingComponent>(TEXT("CharSlideComp"));

	// Gameplay Tag Component.
	CharacterGameplayTag = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("CharContainerTagComp"));

	// Interactive Component.
	InteractiveComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractiveComp"));

	// Pushing Component.
	PushingComponent = CreateDefaultSubobject<UPushingComponent>(TEXT("PushingComp"));

	// EquipmentComponent
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipComp"));

	// Stat Component.
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterSliding->OnInitSlidingComponent(this);
	PushingComponent->OnInitPushingComponent(this);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetActorMovementEnabled_Implementation(bool bEnabled)
{
	if (GetController() != nullptr)
	{
		IControlInterface::Execute_SetActorMovementEnabled(GetController(), bEnabled);
	}
}

void APlayerCharacter::SetActorLookEnabled_Implementation(bool bEnabled)
{
	if (GetController() != nullptr)
	{
		IControlInterface::Execute_SetActorLookEnabled(GetController(), bEnabled);
	}
}
