
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAddTagToGameplayTagContainer, class AActor*, Actor, FGameplayTagContainer, ToGameplayTagContainer, FGameplayTag, NewTagGameplay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRemoveTagToGameplayTagContainer, class AActor*, Actor, FGameplayTagContainer, FromGameplayTagContainer, FGameplayTag, RemovedTagGameplay);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CALYPSO_DEV_API UGameplayTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGameplayTagComponent(const FObjectInitializer& ObjectInitializer);

public:
	/** ������� �������� ����� ���. */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool TryAddTag(const FGameplayTag NewTag);

	/** ������� ������� ��������� ���. */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool TryRemoveTag(const FGameplayTag TagToRemove, bool bDeferParentTags = false);

public:
	/***/
	UFUNCTION(BlueprintCallable, Category = "Utility")
	FORCEINLINE bool ActorHasTag(const FGameplayTag TagToCheck) const { return ActorTagContainer.HasTag(TagToCheck); };

	/***/
	UFUNCTION(BlueprintCallable, Category = "Utility")
	FORCEINLINE bool ActorHasAnyTag(const FGameplayTagContainer TagToCheck) const { return ActorTagContainer.HasAny(TagToCheck); };

	/***/
	UFUNCTION(BlueprintCallable, Category = "Utility")
	FORCEINLINE bool ActorHasTagExact(const FGameplayTagContainer TagToCheck) const { return ActorTagContainer.HasAnyExact(TagToCheck); };

public:
	/** ������� ����� �������� ����� ��� ������. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Added Tag To Container"))
	FOnAddTagToGameplayTagContainer OnActorAddedTagToContainer;

	/** ������� ����� ������ ��� �� ������. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Removed Tag From Container"))
	FOnRemoveTagToGameplayTagContainer OnActorRemovedTagFromContainer;

protected:
	virtual void BeginPlay() override;

private:
	/** ��������� ����� ������. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Actor Tags")
	FGameplayTagContainer ActorTagContainer;
};
