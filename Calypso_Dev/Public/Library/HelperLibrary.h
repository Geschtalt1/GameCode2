
#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelperLibrary.generated.h"

/** Возращает ForDuration, если дебаг включен. */
#define IS_DRAW_DEBUG(bEnabled) (bEnabled != false) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None

/**
 * 
 */
UCLASS()
class CALYPSO_DEV_API UHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** Возращает название слота настроек. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Helper)
	static FString GetSettingSlotName() { return "setting_slot"; }

	/** Возращает угол поверхности, на которой находится актор. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Helper)
	static float GetActorAngleFloor(const AActor* InputActor, FVector OffsetTrace = FVector(0.f, 0.f, 100), ECollisionChannel Channel = ECollisionChannel::ECC_Visibility);

	/** Возращает тип поверхности, на которой находится актор. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Helper)
	static EPhysicalSurface GetActorSurfaceFloor(AActor* InputActor, float ZOffset, ETraceTypeQuery Channel);

	/** Convert Normal To F Vector - Only Yaw */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Helper)
	static FVector NormalToVector(FVector Normal);

	/** Находит ближайших акторов в радиусе. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Helper)
	static bool FindNearestActors(const FVector& Origin, const TArray<AActor*>& InActors, TArray<AActor*>& OutActors, float Radius, float Tolerance = 0.f, int32 MaxResult = 1);
};
