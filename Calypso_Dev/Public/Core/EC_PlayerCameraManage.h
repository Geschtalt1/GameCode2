
#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Interface/PostProcessingInterface.h"
#include "EC_PlayerCameraManage.generated.h"

/**
 * 
 */
UCLASS()
class CALYPSO_DEV_API AEC_PlayerCameraManage : public APlayerCameraManager, public IPostProcessingInterface
{
	GENERATED_BODY()

public:
    virtual FPostProcessSettings BPI_GetPostProcess_Implementation() const override;
    virtual void BPI_SetPostProccess_Implementation(FPostProcessSettings NewPostProccess) override;
	
public:
    /** Пост обработка. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Proccess")
    FPostProcessSettings PostProcessSetting;

    /** Альфа канал пост обработки. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Proccess")
    float PostProcessWeight = 1.f;

public:
    virtual void ApplyCameraModifiers(float DeltaTime, FMinimalViewInfo& InOutPOV) override;
};
