
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableSystemInterface.h"
#include "InteractiveActor.generated.h"

UCLASS(Abstract, meta = (DisplayName = "Interactive Base"))
class CALYPSO_DEV_API AInteractiveActor : public AActor, public IInteractableSystemInterface
{
	GENERATED_BODY()
	
public:	
	AInteractiveActor(const FObjectInitializer& ObjectInitializer);

public:
	/**
	 * The world location in which the interaction will take place.
	 *
	 * The default implementation may be overridden or extended by implementing the custom GetInteractionTransform event in Blueprints.
	 *
	 * @return the transformation near the interactive object in which the interaction will take place.
	 */
	UFUNCTION(BlueprintCallable, Category = InteractiveActor)
	FTransform GetInteractionTransform() const;

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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/**
	 * Order a pawn to move to the interaction location.
	 * The default implementation may be overridden or extended by implementing the custom BeginMoveToInteractive event in Blueprints.
	*/
	UFUNCTION(BlueprintCallable, Category = InteractiveActor)
	void MoveToInteractive(APawn* Pawn);

	/**
	 * Command the pawn to turn to the interaction location.
	*/
	UFUNCTION(BlueprintCallable, Category = InteractiveActor)
	void TurnToInteractive(APawn* Pawn);

	/** Отменяет движение к интерактивному объекту. */
	void CancelMove(APawn* Pawn);

	/***/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Find to Turn in Place"), Category = InteractiveActor)
	bool FindToTurnInPlaceInternal(AActor* Actor, FRotator& OutRotation, UAnimMontage*& OutMontage, float TurnThreshold = 45.f) const;

	/**
	 * Customizable event the world location in which the interaction will take place.
	 *
	 * @return the transformation near the interactive object in which the interaction will take place.
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Get Interaction Transform"), Category = InteractiveActor)
	FTransform GetInteractionTransformInternal() const;
	virtual FTransform GetInteractionTransformInternal_Implementation() const;

protected:
	/**
	  * Customizable event to order a pawn to move to the interaction location.
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Begin Move To Interactive"), Category = InteractiveActor)
	void OnBeginMoveToInteractive(APawn* Pawn);

	/**
	  * Customizable event to check if the actor can pushing in the current state.
	  *
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "End Move To Interactive"), Category = InteractiveActor)
	void OnEndMoveToInteractive(APawn* Pawn);

private:
	/** Возращает true, если указанный актор в радиусе принятия взаимодействия. */
	bool CheckAcceptRadius(AActor* Actor) const;

	/***/
	bool CheckVelocity(APawn* Pawn) const;

	/***/
	bool FindToTurnInPlace(AActor* Actor, FRotator& OutRotation, float& OutAngle, float TurnThreshold = 45.f) const;

private:
	/** Вызывается когда */
	UFUNCTION()
	void OnAddMovement(APawn* Pawn);

	/** Вызывается когда функция поворота была завершена. */
	UFUNCTION()
	void OnFinishTurn(APawn* Pawn);

private:
	/** Радиус принятия. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0"), Category = "Interactive")
	float AcceptRadius = 1.f;

	/***/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Interactive")
	FTransform InteractionOffset;

	/** Timer function OnAddMovement. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Interactive")
	FTimerHandle MoveTimer;
};
