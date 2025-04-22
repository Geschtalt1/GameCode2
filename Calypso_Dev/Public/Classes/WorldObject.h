
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldObject.generated.h"

/**
 * "Костыль" для использования статический функций с WorldContext в производных класса от этого Object. 
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class CALYPSO_DEV_API UWorldObject : public UObject
{
	GENERATED_BODY()
	
};
