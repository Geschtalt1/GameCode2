
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PushingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdatePushedEnabled, class AActor*, Actor, class APushedActor*, Target, bool, bEnabled);

DEFINE_LOG_CATEGORY_STATIC(LogPushing, All, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CALYPSO_DEV_API UPushingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPushingComponent(const FObjectInitializer& ObjectInitializer);

public:
	/** Инициализирует компонент. */
	UFUNCTION(BlueprintCallable, Category = "Pushing|Init")
	void OnInitPushingComponent(ACharacter* InCharacter);

	/** Пытается запустить толкание актора.*/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Action")
	void TryStartPushed(class APushedActor* InPushedActor);

	/** Пытается остановить толкание актора. */
	UFUNCTION(BlueprintCallable, Category = "Pushing|Action")
	bool TryStopPushed();

public:
	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Search")
	void SearchImpactLocation(FVector& OutImpactV1, FVector& OutImpactV2);

	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Search")
	float SearchRelativeRotation(const FVector& InV1, const FVector& InV2, FRotator& OutRelativeRotation);

	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Search")
	bool SearchGroundDistance(float InRadius, float& OutDistanceGround);

	/****/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Utility")
	FVector GetCapsuleBaseLocation(float ZOffset) const;

	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Utility")
	float CalculateUpGroundAngle(FVector Axis, float Scale) const;

public:
	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Utility")
	float CalculateGroundAngle(FTwoVectors& OutTargetBoxTrace, float& OutOffset, float InCapsuleOffset = 10.f);

	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Utility")
	FTransform SetOriginLocation(const FTwoVectors& InTargetBoxTrace, float InOffset);

	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Utility")
	bool GetGroundZLocation(const FTransform& InTransformOrigin, FVector& OutGroundLocation);

	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Utility")
	FTransform GetFixZLocation(const FTransform& InTransformOrigin, const FVector& OutGroundLocation);

	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Utility")
	FTransform SetOriginRotation(const FTransform& InTransformOrigin);

	/***/
	UFUNCTION(BlueprintCallable, Category = "Pushing|Utility")
	FHitResult SetActorLocationAndRotation(const FTransform& InputTransform, float DeltaSeconds);

public:
	/** Возращает true, если толкание актора активно. */
	FORCEINLINE bool IsPushing() const { return (bool)bIsPushingEnabled; }

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Pushed Update Enabled"))
	FOnUpdatePushedEnabled OnActorUpdatePushedEnabled;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** Разворачивает капсулу в сторону толкаемого актора. */
	void RotateCapsuleToLookingActor();

	/** Очищает кешь. */
	void ClearPushedCash();

	/** Обновление толкаемого актора, когда толкание активно. */
	void UpdatePushing(float DeltaSeconds);
	void UpdateOwnerRotation(float DeltaSeconds);

	/** Рассчитывает положение рук на акторе. */
	void SetIKRootRotation();
	void SetIKLocation(float CapsuleOffset = 10.f);
	FTransform CalculateTranformIK(class UArrowComponent* InHandIK);

	/** Проверяет условия выполнения логики толкания. */
	bool CheckActorCondition();

private:
	/** Вызывается когда поврот капсулы завершен. */
	UFUNCTION()
	void OnMoveCompleted();

private:
	/** Тип интерактивного трейса. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	TEnumAsByte<ETraceTypeQuery> PushingTraceType;

	/** Дэбаг компонента. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	uint8 bDebug : 1 = false;

private:
	/** Актор с которым взаимодействуют. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	TObjectPtr<class APushedActor> PushedActor = nullptr;

	/** Владельца компонента. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	TObjectPtr<class ACharacter> Character = nullptr;

	/** Капсула владельца компонента. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	TObjectPtr<class UCapsuleComponent> CapsuleComponent = nullptr;

	/** Мувемент компонент владельца. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	TObjectPtr<class UCharacterMovementComponent> CharacterMovement = nullptr;

	/** Статик мешь толкаемого объекта. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	TObjectPtr<class UStaticMeshComponent> PushedMeshComponent = nullptr;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	FRotator TargetBeginingRotation = FRotator();

	/****/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	FRotator RelativeRotation = FRotator();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	float DistanceToGround = 0.f;

	/****/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	float ObjectRadius = 0.f;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	FTransform LeftHandIK = FTransform();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	FTransform RightHandIK = FTransform();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	uint8 bIsPushingEnabled : 1 = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Pushing")
	float PoseType = 0.f;
		
};
