
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ControlInterface.h"
#include "PlayerCharacter.generated.h"

UCLASS(Abstract)
class CALYPSO_DEV_API APlayerCharacter : public ACharacter, public IControlInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

public:
	virtual void SetActorMovementEnabled_Implementation(bool bEnabled) override;
	virtual void SetActorLookEnabled_Implementation(bool bEnabled) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

private:
	/** Character Sliding Component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UCharacterSlidingComponent> CharacterSliding;

	/** Character Gameplay Tags. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UGameplayTagComponent> CharacterGameplayTag;

	/** Interactive Component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UInteractionComponent> InteractiveComponent;

	/** Pushing Component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UPushingComponent> PushingComponent;

	/** Equipment Component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UEquipmentComponent> EquipmentComponent;

	/** Attribute Component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	TObjectPtr<class UAttributeComponent> AttributeComponent;
};
