
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TIPInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTIPInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Turn In Place Interface.
 */
class CALYPSO_DEV_API ITIPInterface
{
	GENERATED_BODY()

public:
	/*
	 * Stores character turn animations.
	 * Map key is the rotation angle in which the animation for the required rotation is located.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "BPI Get Actor Turn Animations"), Category = "TIP")
	TMap<float, UAnimMontage*> GetActorTurnAnimations();

	/** Play a Montage. Returns Length of Montage in seconds. Returns 0.f if failed to play. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "BPI Try Actor Play Anim In Turn"), Category = "TIP")
	float TryActorPlayAnimTurnInPlace(float Angle);
};
