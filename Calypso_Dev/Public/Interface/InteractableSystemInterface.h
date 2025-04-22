
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/InteractionComponent.h"
#include "InteractableSystemInterface.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReceiveActorInteractionResult);

UINTERFACE(MinimalAPI, BlueprintType)
class UInteractableSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CALYPSO_DEV_API IInteractableSystemInterface
{
	GENERATED_BODY()

public:
	/** Пробует запустить взаимодейстиве актора. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction|Action")
	bool TryActorStartInteraction(AActor* Instigiator);

	/** Возращает true, если с актором можно взаимодействовать. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction|State")
	bool IsCanActorInteraction() const;

	/** Возращает true, если в текущий момент актор взаимодействует. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction|State")
	bool IsActorInteraction() const;

	/** Возращает настройки интеракции. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction|State")
	struct FInteractOption GetActorInteractOption() const;

	/** Вызывается когда актор был обнаружен. */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Detected")
	void OnActorBeginDetected(AActor* Instigiator);

	/** Вызывается когда актор перестал быть обнаружунным. */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Detected")
	void OnActorEndDetected(AActor* Instigiator);

	/** Вызывается когда актор начал взаимодействие, вследствие успешного вызова TryActorStartInteraction(). */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Delegate")
	void OnActorBeginInteraction(AActor* Instigiator);

	/** Вызывается когда актор прекратил взаимодействие. */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Delegate")
	void OnActorEndInteraction(AActor* Instigiator);

public:
	/** Делегат окончания интеракции над предметом. */
	FOnReceiveActorInteractionResult OnReceiveActorEndInteraction;
};
