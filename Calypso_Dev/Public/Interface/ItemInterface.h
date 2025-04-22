
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CALYPSO_DEV_API IItemInterface
{
	GENERATED_BODY()

public:
	/***/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "BPI Get Item Data"), Category = "Item|Action")
	class UItemData* GetItemData() const;
};
