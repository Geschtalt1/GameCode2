
#pragma once

#include "CoreMinimal.h"
#include "Classes/InteractiveActor.h"
#include "NarrowPassageBase.generated.h"

UCLASS(Abstract)
class CALYPSO_DEV_API ANarrowPassageBase : public AInteractiveActor
{
	GENERATED_BODY()
	
public:	
	ANarrowPassageBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
