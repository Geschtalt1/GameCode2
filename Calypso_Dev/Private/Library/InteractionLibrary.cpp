

#include "Library/InteractionLibrary.h"

#include "Components/InteractionComponent.h"

#include "Interface/InteractableSystemInterface.h"

void UInteractionLibrary::FinishInteraction(AActor* InActor)
{
	if (auto Interact = Cast<IInteractableSystemInterface>(InActor))
	{
		Interact->OnReceiveActorEndInteraction.Broadcast();
	}
}

UInteractionComponent* UInteractionLibrary::GetInteractionComponentFromActor(AActor* FromActor)
{
	if (FromActor != nullptr)
	{
		return FromActor->GetComponentByClass<UInteractionComponent>();
	}
	return nullptr;
}