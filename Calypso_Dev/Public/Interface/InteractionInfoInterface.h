
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInfoInterface.generated.h"

USTRUCT(BlueprintType)
struct FInteractionInfo
{
	GENERATED_BODY()

public:
	/** Название актора. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ActorDisplayName;

	/** Название действия. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ActionDisplayName;
};

UINTERFACE(MinimalAPI)
class UInteractionInfoInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CALYPSO_DEV_API IInteractionInfoInterface
{
	GENERATED_BODY()

public:
	/***/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "BPI Get Actor Interaction Info"), Category = "Interaction|Info")
	FInteractionInfo GetActorInteractionInfo() const;

	/***/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "BPI Set Actor Interaction Info"), Category = "Interaction|Info")
	void SetActorInteractionInfo(FInteractionInfo InInfo);
};
