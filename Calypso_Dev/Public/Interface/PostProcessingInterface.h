
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PostProcessingInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPostProcessingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CALYPSO_DEV_API IPostProcessingInterface
{
	GENERATED_BODY()

public:
	/** �������� ������� ���� ���������. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Post Process")
	FPostProcessSettings BPI_GetPostProcess() const;

	/** ������������� ����� ���� ���������. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Post Process")
	void BPI_SetPostProccess(FPostProcessSettings NewPostProccess);
};
