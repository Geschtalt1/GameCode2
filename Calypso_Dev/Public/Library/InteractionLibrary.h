
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CALYPSO_DEV_API UInteractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** Прекращает взаимодествие актора. */
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = InActor), Category = "Interaction Library")
	static void FinishInteraction(AActor* InActor);

	/** Возращает указатель на InteractionComponent из указаного актора. */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Interaction System Component"), Category = "Interaction Library")
	static class UInteractionComponent* GetInteractionComponentFromActor(AActor* FromActor);
};
