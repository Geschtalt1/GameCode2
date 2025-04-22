

#include "Core/EC_PlayerCameraManage.h"

void AEC_PlayerCameraManage::ApplyCameraModifiers(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	Super::ApplyCameraModifiers(DeltaTime, InOutPOV);

	AddCachedPPBlend(PostProcessSetting, PostProcessWeight);
}

void AEC_PlayerCameraManage::BPI_SetPostProccess_Implementation(FPostProcessSettings NewPostProccess)
{
	PostProcessSetting = NewPostProccess;
}

FPostProcessSettings AEC_PlayerCameraManage::BPI_GetPostProcess_Implementation() const
{
	return PostProcessSetting;
}