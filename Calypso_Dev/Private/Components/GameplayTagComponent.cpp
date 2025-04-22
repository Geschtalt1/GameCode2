

#include "Components/GameplayTagComponent.h"

#include "GameplayTagContainer.h"

UGameplayTagComponent::UGameplayTagComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

}

void UGameplayTagComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UGameplayTagComponent::TryAddTag(const FGameplayTag NewTag)
{
	if (ActorHasTagExact(FGameplayTagContainer(NewTag)) != true)
	{
		ActorTagContainer.AddTag(NewTag);
		OnActorAddedTagToContainer.Broadcast(GetOwner(), ActorTagContainer, NewTag);
		
		return true;
	}
	return false;
}

bool UGameplayTagComponent::TryRemoveTag(const FGameplayTag TagToRemove, bool bDeferParentTags)
{
	if (ActorHasTagExact(FGameplayTagContainer(TagToRemove)) != false)
	{
		if (ActorTagContainer.RemoveTag(TagToRemove, bDeferParentTags) != false)
		{
			OnActorRemovedTagFromContainer.Broadcast(GetOwner(), ActorTagContainer, TagToRemove);
			return true;
		}
	}
	return false;
}
