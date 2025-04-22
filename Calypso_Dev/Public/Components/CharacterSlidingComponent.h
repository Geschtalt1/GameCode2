
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
	/** ��������� true, ���� �������� �������� ���������. */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool IsCanStartSliding() const;

	/** ��������� true, ���� �������� ����� ����������. */
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

	/** ������� ��������� ��������. */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool TryStartSliding();

	/** ������� ���������� ��������. */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool TryStopSliding();

public:
	/** ����� �����������. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding")
	TEnumAsByte<ETraceTypeQuery> TraceDetectedGroundType;
	
	/** �����. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding|Debug")
	TEnumAsByte <EDrawDebugTrace::Type> DrawDebugTrace = EDrawDebugTrace::None;

	/** ������� ������ ���������. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Character Sliding Start"))
	FOnCharacterSliding OnCharacterSlidingStart;
	
	/** ������� ����������� ���������. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Character Sliding End"))
	FOnCharacterSliding OnCharacterSlidingEnd;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** ���������� ����� �������� ��� �������. */
	void OnStartSliding();

	/** ���������� ����� �������� ��� ��������. */
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
	/** ��������� ���������. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding", meta = (AllowPrivateAccess = "true"))
	FSlidingOption SlidingOption;

	/** ���� ���������� ������������. */
	UPROPERTY(BlueprintReadOnly, Category = "Sliding", meta = (AllowPrivateAccess = "true"))
	FSlidingOption CashMovementOption;

	/** ������ �� ���������. */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACharacter> Character;

	/** ������ �� ��������� �������. */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	/** ������ �� ����. */
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	/** ������ �� ���� ��������. */
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimInstance> AnimInstance;

	/** ������ �� �������� ��������. */
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
