

#include "Classes/InteractiveActor.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "Interface/ControlInterface.h"
#include "Interface/TIPInterface.h"

AInteractiveActor::AInteractiveActor(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform AInteractiveActor::GetInteractionTransform() const
{
	return GetInteractionTransformInternal() + InteractionOffset;
}

FTransform AInteractiveActor::GetInteractionTransformInternal_Implementation() const
{
	return FTransform();
}

void AInteractiveActor::MoveToInteractive(APawn* Pawn)
{
	if (Pawn != nullptr)
	{
		// Start timer.
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AInteractiveActor::OnAddMovement, Pawn);
		GetWorld()->GetTimerManager().SetTimer(MoveTimer, TimerDelegate, 0.001f, true);

		// Disabled movement in pawn.
		if (Pawn->Implements<UControlInterface>() != false)
		{
			IControlInterface::Execute_SetActorMovementEnabled(Pawn, false);
		}

		OnBeginMoveToInteractive(Pawn);
	}
}

void AInteractiveActor::OnAddMovement(APawn* Pawn)
{
	if (CheckAcceptRadius(Pawn) != true)
	{
		// Find look rotation.
		const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(
			Pawn->GetActorLocation(),
			GetInteractionTransform().GetLocation()
		);
		
		// Find direction movement.
		const FVector WorldDirection = UKismetMathLibrary::GetForwardVector(Rotation);
		Pawn->AddMovementInput(WorldDirection, 1.0f, true);

		if (CheckVelocity(Pawn) != false)
		{
			GetWorldTimerManager().ClearTimer(MoveTimer);
			TurnToInteractive(Pawn);
		}
	}
	else
	{
		// Cancel move.
		GetWorldTimerManager().ClearTimer(MoveTimer);
		TurnToInteractive(Pawn);
	}
}

void AInteractiveActor::CancelMove(APawn* Pawn)
{
	if (Pawn != nullptr)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
		OnEndMoveToInteractive(Pawn);
	}
}

void AInteractiveActor::TurnToInteractive(APawn* Pawn)
{
	if (Pawn != nullptr)
	{
		FRotator RotLocal;
		float Angle;
		if (FindToTurnInPlace(Pawn, RotLocal, Angle) != false)
		{
			// Play Animation.
			if (Pawn->Implements<UTIPInterface>() != false)
			{
				const float Duration = ITIPInterface::Execute_TryActorPlayAnimTurnInPlace(Pawn, Angle);

				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AInteractiveActor::OnFinishTurn, Pawn);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Duration, false);
			}
			else
			{
				OnFinishTurn(Pawn);
			}
		}
		else
		{
			OnFinishTurn(Pawn);
		}
	}
}

bool AInteractiveActor::CheckAcceptRadius(AActor* Actor) const
{
	if (Actor != nullptr)
	{
		const float Distance = FVector::Distance(
			Actor->GetActorLocation(),
			GetInteractionTransform().GetLocation()
		);

		return Distance <= AcceptRadius;
	}
	return false;
}

bool AInteractiveActor::CheckVelocity(APawn* Pawn) const
{
	if (Pawn != nullptr)
	{
		if (auto CharMovement = Pawn->GetComponentByClass<UCharacterMovementComponent>())
		{
			const float LenghtAcceleration = CharMovement->GetCurrentAcceleration().Size2D();
			const float LenghtVelocty = Pawn->GetVelocity().Size2D();

			return (LenghtAcceleration > 0.f) && (LenghtVelocty < 3.f);
		}
	}
	return false;
}

bool AInteractiveActor::FindToTurnInPlaceInternal(AActor* Actor, FRotator& OutRotation, UAnimMontage*& OutMontage, float TurnThreshold) const
{
	float Angle;
	if (FindToTurnInPlace(Actor, OutRotation, Angle, TurnThreshold))
	{
		// Check there a interface.
		if (Actor->Implements<UTIPInterface>())
		{
			// Get animations turn from actor.
			auto TurnAnims = ITIPInterface::Execute_GetActorTurnAnimations(Actor);
			if (TurnAnims.Contains(Angle) != false)
			{
				OutMontage = *TurnAnims.Find(Angle);

				return OutMontage != nullptr;
			}
		}
	}
	return false;
}

bool AInteractiveActor::FindToTurnInPlace(AActor* Actor, FRotator& OutRotation, float& OutAngle, float TurnThreshold) const
{
	if (Actor != nullptr)
	{
		// Get actor rotation Z.
		const FRotator OriginActorRotation = Actor->GetActorRotation();
		const float ActorRotZ = OriginActorRotation.Yaw;

		// Get delta rotation.
		const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(
			GetInteractionTransform().Rotator(),
			OriginActorRotation
		);

		const float LocalRotationZ = DeltaRotation.Yaw;
		OutRotation = FRotator(0.f, ActorRotZ + LocalRotationZ, 0.f);

		if (UKismetMathLibrary::InRange_FloatFloat(LocalRotationZ, TurnThreshold, 135.f))
		{
			OutAngle = 90.f; // Turn right 90.
		}
		else if (UKismetMathLibrary::InRange_FloatFloat(LocalRotationZ, 135.f, 180.f))
		{
			OutAngle = 180.f; // Turn right 180.
		}
		else if (UKismetMathLibrary::InRange_FloatFloat(LocalRotationZ, -135, (TurnThreshold * (-1.f))))
		{
			OutAngle = -90.f; // Turn left 90.
		}
		else
		{
			OutAngle = -180.f;  // Turn left 180.
		}

		return UKismetMathLibrary::Abs(LocalRotationZ) > TurnThreshold;
	}
	return false;
}

void AInteractiveActor::OnFinishTurn(APawn* Pawn)
{
	// Enabled movement in pawn.
	if (Pawn->Implements<UControlInterface>() != false)
	{
		IControlInterface::Execute_SetActorMovementEnabled(Pawn, true);
	}

	OnEndMoveToInteractive(Pawn);

	UE_LOG(LogInteraction, Display, TEXT("Finish Turn Pawn: %s"), *Pawn->GetName());
}

/********************************************************** FUNCTION INTERFACES ************************************************************************************/

bool AInteractiveActor::TryActorStartInteraction_Implementation(AActor* Instigiator)
{
	return false;
}

bool AInteractiveActor::IsCanActorInteraction_Implementation() const
{
	return false;
}

bool AInteractiveActor::IsActorInteraction_Implementation() const
{
	return false;
}

void AInteractiveActor::OnActorBeginDetected_Implementation(AActor* Instigiator)
{
}

void AInteractiveActor::OnActorEndDetected_Implementation(AActor* Instigiator)
{
}

void AInteractiveActor::OnActorBeginInteraction_Implementation(AActor* Instigiator)
{
}

void AInteractiveActor::OnActorEndInteraction_Implementation(AActor* Instigiator)
{
}

FInteractOption AInteractiveActor::GetActorInteractOption_Implementation() const
{
	return FInteractOption();
}
