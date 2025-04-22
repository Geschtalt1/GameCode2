

#include "Components/CharacterSlidingComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

UCharacterSlidingComponent::UCharacterSlidingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCharacterSlidingComponent::OnInitSlidingComponent(ACharacter* Owner)
{
	if (Owner != nullptr)
	{
		Character = Owner;
		Mesh = Owner->GetMesh();
		CapsuleComponent = Owner->GetCapsuleComponent();
		CharacterMovement = Owner->GetCharacterMovement();
		AnimInstance = Mesh->GetAnimInstance();
	}
}

void UCharacterSlidingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UCharacterSlidingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TryStartSliding();
	OnAddWordlOffsetLeftRight();
	TryStopSliding();
	
	UpdateSliding();
}

void UCharacterSlidingComponent::UpdateSliding()
{
	FHitResult Result;
	bIsDetectedGround = CreateDetectionLineTrace(Result);

	if (bIsDetectedGround != false)
	{
		for (int32 i = 0; i < 6; i++)
		{
			if (GroundWallsDetection(i) != true)
			{
				break;
			}
		}

		OnSetCanSlidingValue();
	}
}

void UCharacterSlidingComponent::OnSetCanSlidingValue()
{
	bIsCanSliding = (ForwardGroundPoints.Num() > 5) && (CharacterMovement->IsFalling() != false);
	ForwardGroundPoints.Empty();
}

void UCharacterSlidingComponent::OnAddWordlOffsetLeftRight()
{
	if ((bIsSliding != false) && (bIsDetectedGround != false))
	{
		// Get right vector character.
		const FVector RightVector = GetOwner()->GetActorRightVector();

		// Add offset.
		const FVector DeltaOffset = RightVector * (AxisInterp.Y * 0.02f);
		GetOwner()->AddActorWorldOffset(DeltaOffset);
	}
}

bool UCharacterSlidingComponent::IsCanStartSliding() const
{
	return (bIsSliding != true) && (bIsDetectedGround != false)  && (bIsCanSliding != false);
}

bool UCharacterSlidingComponent::IsCanStopSliding() const
{
	return (bIsSliding != false) && ((bIsDetectedGround != true) || (bIsCanSliding != true));
}

bool UCharacterSlidingComponent::TryStartSliding()
{
	if (IsCanStartSliding())
	{
		OnStartSliding();
		return true;
	}
	return false;
}

bool UCharacterSlidingComponent::TryStopSliding()
{
	if (IsCanStopSliding())
	{
		OnEndSliding();
		return true;
	}
	return false;
}

void UCharacterSlidingComponent::OnStartSliding()
{
	bIsSliding = true;

	SaveCashMovement();

	if (CapsuleComponent != nullptr)
	{
		// Cash Scaled Capsule Size;
		float Radius, HalfHeight;
		CapsuleComponent->GetScaledCapsuleSize(Radius, HalfHeight);
		CapsuleSize.X = Radius;
		CapsuleSize.Y = HalfHeight;

		// Set new value half height.
		CapsuleComponent->SetCapsuleHalfHeight(60.f);
	}

	SetMovementOption(SlidingOption);

	// Call delegate.
	OnCharacterSlidingStart.Broadcast(Character);
}

void UCharacterSlidingComponent::OnEndSliding()
{
	bIsSliding = false;

	if (CapsuleComponent != nullptr)
	{
		// Set old value half height.
		CapsuleComponent->SetCapsuleHalfHeight(CapsuleSize.Y);
	}

	SetMovementOption(CashMovementOption);

	// Call delegate.
	OnCharacterSlidingEnd.Broadcast(Character);
}

bool UCharacterSlidingComponent::CreateDetectionLineTrace(FHitResult& Result)
{
	if ((CapsuleComponent != nullptr) && (CharacterMovement !=nullptr))
	{
		const float CapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
		const FVector OwnerLocation = GetOwner()->GetActorLocation();
		const FVector OwnerFwd = GetOwner()->GetActorForwardVector();

		// Calculate start trace & end trace.
		const FVector Start = (OwnerLocation + (OwnerFwd * 2.f) - FVector(0.f, 0.f, CapsuleHalfHeight * 0.8f)) + FVector(0.f, 0.f, 5.f);
		const FVector End = Start + FVector(0.f, 0.f, (bIsSliding ? -65.f : -45.f));

		/** Create Line Trace. */
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetOwner(),
			Start,
			End,
			TraceDetectedGroundType,
			false,
			{ GetOwner() },
			DrawDebugTrace,
			Result,
			true
		);

		if (bHit != false)
		{
			if (CharacterMovement->IsWalkable(Result) != true)
			{
				// Get Velocity Owner.
				const FVector Velocity = GetOwner()->GetVelocity();
				const FVector2D Velocity2D = FVector2D(Velocity.X, Velocity.Y);
				const bool bFalling = CharacterMovement->IsFalling();

				// Check detection ground and return result.
				return (UKismetMathLibrary::VSize2D(Velocity2D) > 0.f) && 
					((Velocity.Z < 0.f) && (UKismetMathLibrary::Abs(Velocity.Z) > 150.f)) && (bFalling != false);
			}
		}
	}

	return false;
}

bool UCharacterSlidingComponent::GroundWallsDetection(int32 Index)
{
	const float CapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const FVector OwnerFwd = GetOwner()->GetActorForwardVector();
	FHitResult Result;

	FTwoVectors SelectedTwoVectors;
	const FVector VectorBase = (OwnerLocation + (OwnerFwd * (UKismetMathLibrary::Conv_IntToFloat(Index) * 20.f))) - FVector(0.f, 0.f, CapsuleHalfHeight * 0.8f);
	if (ForwardGroundPoints.Num() > 0)
	{
		// Get index.
		const int32 IndexLocal = UKismetMathLibrary::Clamp(ForwardGroundPoints.Num() - 1, 0, 11);

		// Calculate first the pair vectors.
		const FVector VectorChild = FVector(VectorBase.X, VectorBase.Y, ForwardGroundPoints[IndexLocal].v1.Z);
		SelectedTwoVectors = FTwoVectors(VectorChild + FVector(0.f, 0.f, 35.f), VectorChild + FVector(0.f, 0.f, -55.f));
	}
	else
	{
		// Calculate second the pair vectors.
		SelectedTwoVectors = FTwoVectors(VectorBase + FVector(0.f, 0.f, 5.f), VectorBase + FVector(0.f, 0.f, -70.f));
	}

	/** Create Line Trace. */
	const bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetOwner(),
		SelectedTwoVectors.v1,
		SelectedTwoVectors.v2,
		TraceDetectedGroundType,
		false,
		{ GetOwner() },
		DrawDebugTrace,
		Result,
		true
	);

	if (bHit != false)
	{
		// Get new local index.
		const int32 IndexLocal = UKismetMathLibrary::Clamp(ForwardGroundPoints.Num() - 1, 0, 10);
		const bool bSuccessfully = (ForwardGroundPoints.Num() > 0) ? (ForwardGroundPoints[IndexLocal].v1.Z > Result.ImpactPoint.Z) : true;

		if (bSuccessfully != false)
		{
			ForwardGroundPoints.Add(FTwoVectors(Result.ImpactPoint, Result.ImpactNormal));

			return true;
		}
	}

	return false;
}

void UCharacterSlidingComponent::CreateAxisValuesWithInterp(FVector2D InputAxis, float InterpSpeed)
{
	// Cash delta second.
	const float DeltaSecondsLocal = UGameplayStatics::GetWorldDeltaSeconds(GetOwner());

	// Interp Right-Left.
	AxisInterp.X = UKismetMathLibrary::FInterpTo(AxisInterp.X, InputAxis.X, DeltaSecondsLocal, InterpSpeed);

	// Interp Forward-Backward.
	AxisInterp.Y = UKismetMathLibrary::FInterpTo(AxisInterp.Y, InputAxis.X, DeltaSecondsLocal, InterpSpeed);
}

void UCharacterSlidingComponent::SetMovementOption(const FSlidingOption& Option)
{
	if (CharacterMovement != nullptr)
	{
		CharacterMovement->AirControl = Option.AirControl;
		CharacterMovement->GravityScale = Option.GravityScale;
	}
}

void UCharacterSlidingComponent::SaveCashMovement()
{
	if (CharacterMovement != nullptr)
	{
		CashMovementOption.AirControl = CharacterMovement->AirControl;
		CashMovementOption.GravityScale = CharacterMovement->GravityScale;
	}
}