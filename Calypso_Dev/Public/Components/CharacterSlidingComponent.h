
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterSlidingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSliding, class ACharacter*, Character);

USTRUCT(BlueprintType)
struct FSlidingOption
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirControl;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CALYPSO_DEV_API UCharacterSlidingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterSlidingComponent();

public:
	UFUNCTION(BlueprintCallable, Category = "Init")
	void OnInitSlidingComponent(class ACharacter* Owner);

public:
	/** Возращает true, если слайдинг возможно запустить. */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool IsCanStartSliding() const;

	/** Возращает true, если слайдинг можно остановить. */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool IsCanStopSliding() const;

public:
	/***/
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool CreateDetectionLineTrace(FHitResult& Result);

	/***/
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void CreateAxisValuesWithInterp(FVector2D InputAxis, float InterpSpeed);

	/***/
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool GroundWallsDetection(int32 Index);

	/** Пробует запустить слайдинг. */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool TryStartSliding();

	/** Пробует остановить слайдинг. */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool TryStopSliding();

public:
	/** Канад трассеровки. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding")
	TEnumAsByte<ETraceTypeQuery> TraceDetectedGroundType;
	
	/** Дебаг. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding|Debug")
	TEnumAsByte <EDrawDebugTrace::Type> DrawDebugTrace = EDrawDebugTrace::None;

	/** Делегат начала слайдинга. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Character Sliding Start"))
	FOnCharacterSliding OnCharacterSlidingStart;
	
	/** Делегат законченого слайдинга. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Character Sliding End"))
	FOnCharacterSliding OnCharacterSlidingEnd;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** Вызывается когда слайдинг был запущен. */
	void OnStartSliding();

	/** Вызывается когда слайдинг был закончен. */
	void OnEndSliding();

	/***/
	void OnAddWordlOffsetLeftRight();

	/***/
	void UpdateSliding();

	/***/
	void OnSetCanSlidingValue();

	/***/
	void SetMovementOption(const FSlidingOption& Option);

	/***/
	void SaveCashMovement();

private:
	/** Параметры слайдинга. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding", meta = (AllowPrivateAccess = "true"))
	FSlidingOption SlidingOption;

	/** Кешь параметров передвижение. */
	UPROPERTY(BlueprintReadOnly, Category = "Sliding", meta = (AllowPrivateAccess = "true"))
	FSlidingOption CashMovementOption;

	/** Ссылка на владельца. */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACharacter> Character;

	/** Ссылка на компонент капсулы. */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	/** Ссылка на мешь. */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	/** Ссылка на аним блюпринт. */
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimInstance> AnimInstance;

	/** Ссылка на чарактер мувемент. */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterMovementComponent> CharacterMovement;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TArray<FTwoVectors> ForwardGroundPoints;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FVector2D CapsuleSize;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bIsDetectedGround = false;

	/***/
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bIsSliding = false;

	/***/
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bIsCanSliding = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FVector2D AxisInterp;
		
};
