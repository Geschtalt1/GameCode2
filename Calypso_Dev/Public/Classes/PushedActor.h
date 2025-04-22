
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/InteractableSystemInterface.h"
#include "PushedActor.generated.h"

USTRUCT(BlueprintType)
struct FPushOption
{
	GENERATED_BODY()

public:
	/** Скорость толкания. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PushSpeed = FVector(100.f, 100.f, 100.f);
};

UCLASS(Abstract)
class CALYPSO_DEV_API APushedActor : public AActor, public IInteractableSystemInterface
{
	GENERATED_BODY()
	
public:	
	APushedActor(const FObjectInitializer& ObjectInitializer);

public:
	/** Finds the closest position to push an actor. */
	UFUNCTION(BlueprintCallable, Category = PushedActor)
	int32 FindClosestPushPosition(const FVector& InputActorLocation, float InRange);

	/**d*/
	UFUNCTION(BlueprintCallable, Category = PushedActor)
	bool CheckPermissibleFloorAngle(const FVector& InLocation, float InRadius, float InHalfHeight, float InWalkalableFloorZ);

	/**
	 * Check if the acotr can pushing in the current state.
	 *
	 * The default implementation may be overridden or extended by implementing the custom CanPushing event in Blueprints.
	 *
	 * @Return Whether the actor can pushing in the current state.
	 */
	UFUNCTION(BlueprintCallable, Category = PushedActor)
	bool CanPushing() const;

public:
	/** Возращает статик мешь компонент. */
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return PushedMesh; }

	/** Возращает компонет стрелы для IK рук. */
	FORCEINLINE class UArrowComponent* GetRootIK() const { return HandsIK; }
	FORCEINLINE class UArrowComponent* GetRightIK() const { return RightHandIK; }
	FORCEINLINE class UArrowComponent* GetLeftIK() const { return LeftHandIK; }

public:
	virtual bool TryActorStartInteraction_Implementation(AActor* Instigiator) override;
	virtual bool IsCanActorInteraction_Implementation() const override;
	virtual bool IsActorInteraction_Implementation() const override;
	virtual void OnActorBeginDetected_Implementation(AActor* Instigiator) override;
	virtual void OnActorEndDetected_Implementation(AActor* Instigiator) override;
	virtual void OnActorBeginInteraction_Implementation(AActor* Instigiator) override;
	virtual void OnActorEndInteraction_Implementation(AActor* Instigiator) override;
	virtual FInteractOption GetActorInteractOption_Implementation() const override;

protected:
	/**
	  * Customizable event to check if the actor can pushing in the current state.
	  * 
	  * @Return Whether the actor can pushing in the current state.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = PushedActor, meta = (DisplayName = "CanPushing"))
	bool CanPushingInternal() const;
	virtual bool CanPushingInternal_Implementation() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	/** Visual static mesh. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UStaticMeshComponent> PushedMesh;

	/** Root arrow for hands IK. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UArrowComponent> HandsIK;

	/** Right arrow component hand IK. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UArrowComponent> RightHandIK;

	/** Left arrow component IK. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UArrowComponent> LeftHandIK;

private:
	/** Pushing position. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true", MakeEditWidget = "true"), Category = "Pushing")
	TArray<FTransform> PushTransform;

	/***/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	uint8 bIsDetectedActor : 1;

	/** Настройки толкания объекта. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	FPushOption PushOption;

	/** Канад трассеровки. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	TEnumAsByte<ETraceTypeQuery> TraceType;

	/** Debug. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Pushing|Debug")
	uint8 bDebug:1;
};
