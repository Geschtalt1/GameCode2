
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControlInterface.generated.h"


UINTERFACE(MinimalAPI)
class UControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CALYPSO_DEV_API IControlInterface
{
	GENERATED_BODY()

public:
	/** ������������� ����������� ������������. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "BPI Set Actor Movement Enabled"), Category = "Control|State")
	void SetActorMovementEnabled(bool bEnabled);

	/** ������������� ����������� �������. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "BPI Set Actor Look Enabled"), Category = "Control|State")
	void SetActorLookEnabled(bool bEnabled);
};
