

#include "Components/PushingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Classes/PushedActor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Library/HelperLibrary.h"
#include "Library/InteractionLibrary.h"

#include "NavigationSystem.h"

#include "Logging/LogMacros.h"

UPushingComponent::UPushingComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UPushingComponent::OnInitPushingComponent(ACharacter* InCharacter)
{
	if (InCharacter != nullptr)
	{
		Character = InCharacter;
		CapsuleComponent = InCharacter->GetCapsuleComponent();
		CharacterMovement = InCharacter->GetCharacterMovement();
	}
}


void UPushingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UPushingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePushing(DeltaTime);
}

void UPushingComponent::UpdatePushing(float DeltaSeconds)
{
	if (bIsPushingEnabled != false)
	{
		float OffsetLocal;
		FTwoVectors TargetBoxTrace;
		FVector GroundLocation;

		CalculateGroundAngle(TargetBoxTrace, OffsetLocal);
		FTransform TransformOrigin = SetOriginLocation(TargetBoxTrace, OffsetLocal);

		if (GetGroundZLocation(TransformOrigin, GroundLocation) != true)
		{
			TryStopPushed(); return;
		}

		TransformOrigin = GetFixZLocation(TransformOrigin, GroundLocation);
		TransformOrigin = SetOriginRotation(TransformOrigin);
		SetActorLocationAndRotation(TransformOrigin, DeltaSeconds);

		if (CheckActorCondition() != true)
		{
			UE_LOG(LogPushing, Warning, TEXT("Check condition return FALSE!"));

			TryStopPushed(); return;
		}

		// IK Hands.
		SetIKRootRotation();
		SetIKLocation(10.f);

		UpdateOwnerRotation(DeltaSeconds);
	}
}

bool UPushingComponent::CheckActorCondition()
{
	return (PushedActor != nullptr) ? (PushedActor->CanPushing()) : (false);
}

void UPushingComponent::TryStartPushed(APushedActor* InPushedActor)
{
	if ((InPushedActor != nullptr) && (IsPushing() != true))
	{
		UE_LOG(LogPushing, Display, TEXT("Try Start Pushed for Actor: %s"), *InPushedActor->GetName());

		PushedActor = InPushedActor;
		PushedMeshComponent = InPushedActor->GetMesh();
		RotateCapsuleToLookingActor();
	}
	else
	{
		UE_LOG(LogPushing, Error, TEXT("Failed Try Start Pushed!"));
	}
}

bool UPushingComponent::TryStopPushed()
{
	if (IsPushing() != false)
	{
		UE_LOG(LogPushing, Display, TEXT("Try Stop Pushed Actor: %s"), *PushedActor->GetName());

		bIsPushingEnabled = false;
		SetComponentTickEnabled(false);

		UInteractionLibrary::FinishInteraction(PushedActor);

		// Call delegate.
		OnActorUpdatePushedEnabled.Broadcast(GetOwner(), PushedActor, false);
		ClearPushedCash();

		return true;
	}
	else
	{
		UE_LOG(LogPushing, Error, TEXT("Failed Try Stop Pushed!"));
	}
	return false;
}

FVector UPushingComponent::GetCapsuleBaseLocation(float ZOffset) const
{
	if (CapsuleComponent != nullptr)
	{
		// Get value capsule component.
		const FVector WorldLocation = CapsuleComponent->GetComponentLocation();
		const FVector UpVector = CapsuleComponent->GetUpVector();
		const float ScaledCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();

		return WorldLocation - (UpVector * (ScaledCapsuleHalfHeight + ZOffset));
	}
	return FVector();
}

float UPushingComponent::CalculateUpGroundAngle(FVector Axis, float Scale) const
{
	if ((CharacterMovement != nullptr) && (CapsuleComponent != nullptr))
	{
		// Calculate capsule location.
		const FVector OriginCapsuleLocation = CapsuleComponent->GetComponentLocation();
		const FVector CapsuleLocation = (Axis * Scale) + OriginCapsuleLocation;

		// Find floor.
		FFindFloorResult FloorResult;
		CharacterMovement->FindFloor(CapsuleLocation, FloorResult, false);
		const FHitResult Hit = FloorResult.HitResult;

		const float Multiplay = (GetCapsuleBaseLocation(0.f).Z - Hit.ImpactPoint.Z) > 0.f ? (1.f) : (-1.f);

		// Make rotator from x; 
		const FRotator RotatorFromX = UKismetMathLibrary::MakeRotFromX(Hit.Normal);

		// Dot product.
		const float DotProduct = UKismetMathLibrary::Dot_VectorVector(
			UKismetMathLibrary::GetForwardVector(RotatorFromX),
			GetOwner()->GetActorForwardVector()
		);

		return Multiplay * UKismetMathLibrary::Abs(DotProduct);
	}
	return 0.0f;
}

void UPushingComponent::RotateCapsuleToLookingActor()
{
	if ((PushedActor != nullptr) && (Character != nullptr))
	{
		UE_LOG(LogPushing, Display, TEXT("Start Rotate Capsule To Looking Actor!"));

		FHitResult ResultLocal;

		// Create line trace from character to pushed actor.
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			this,
			Character->GetActorLocation(),
			PushedActor->GetActorLocation(),
			PushingTraceType,
			false,
			{},
			IS_DRAW_DEBUG(bDebug),
			ResultLocal,
			true
		);

		if (bHit != false)
		{
			const FVector PreNormal = UHelperLibrary::NormalToVector(ResultLocal.Normal);
			TargetBeginingRotation = UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::Normal(PreNormal));

			// Capsule rotation to pushed actor.
			UKismetSystemLibrary::MoveComponentTo(
				CapsuleComponent,
				CapsuleComponent->GetComponentLocation(),
				TargetBeginingRotation,
				true,
				true,
				0.2f,
				false,
				EMoveComponentAction::Move,
				FLatentActionInfo(0, 1, TEXT("OnMoveCompleted"), this)
			);
		}
	}

}

void UPushingComponent::OnMoveCompleted()
{
	UE_LOG(LogPushing, VeryVerbose, TEXT("Rotate Completed!"));

	FVector V1, V2;
	SearchImpactLocation(V1, V2);
	ObjectRadius = SearchRelativeRotation(V1, V2, RelativeRotation);

	UE_LOG(LogPushing, Display, TEXT("Object Radius: %f"), ObjectRadius);

	// If return true, then start pushed.
	if (SearchGroundDistance(ObjectRadius, DistanceToGround) != false)
	{
		UE_LOG(LogPushing, Display, TEXT("Pushing Enabled!"));

		bIsPushingEnabled = true;
		PushedMeshComponent->IgnoreActorWhenMoving(GetOwner(), true);
		SetComponentTickEnabled(true);

		// Call delegate.
		OnActorUpdatePushedEnabled.Broadcast(GetOwner(), PushedActor, true);
	}
	else
	{
		UE_LOG(LogPushing, Error, TEXT("Push failed to start!"));

		bIsPushingEnabled = false;
		ClearPushedCash();
	}
}

void UPushingComponent::ClearPushedCash()
{
	ObjectRadius = 0.f;
	DistanceToGround = 0.f;
	PushedActor = nullptr;
	PushedMeshComponent = nullptr;
	RelativeRotation = FRotator();
	TargetBeginingRotation = FRotator();

	UE_LOG(LogPushing, Warning, TEXT("Clear Cash"));
}

void UPushingComponent::SearchImpactLocation(FVector& OutImpactV1, FVector& OutImpactV2)
{
	if ((PushedActor != nullptr) && (PushedMeshComponent != nullptr))
	{
		for (int32 i = 0; i <= 1; i++)
		{
			// Get Location Pushed Actor.
			const FVector OriginLocation = PushedActor->GetActorLocation();

			// Get bounds visual mesh pushed actor.
			FVector Origin, BoxExtern;
			float SpheareRadius;
			UKismetSystemLibrary::GetComponentBounds(PushedMeshComponent, Origin, BoxExtern, SpheareRadius);

			FHitResult Result;

			// Calculate start trace.
			const float Multiplay = (i == 0) ? (0.8f) : (-0.8f);
			const FVector StartLoc = OriginLocation + (UKismetMathLibrary::GetForwardVector(TargetBeginingRotation) * (SpheareRadius * Multiplay));

			// Create line trace.
			UKismetSystemLibrary::LineTraceSingle(
				this,
				StartLoc,
				OriginLocation,
				PushingTraceType,
				false,
				{},
				IS_DRAW_DEBUG(bDebug),
				Result,
				true
			);

			switch (i)
			{
			case 0:
				OutImpactV1 = Result.ImpactPoint; break;
			case 1:
				OutImpactV2 = Result.ImpactPoint; break;
			}
		}
	}
}

float UPushingComponent::SearchRelativeRotation(const FVector& InV1, const FVector& InV2, FRotator& OutRelativeRotation)
{
	if (PushedActor != nullptr)
	{
		// Get pushed actor transform.
		const FTransform ActorPushedTransform = PushedActor->GetActorTransform();
		
		// Make relative transform.
		const FTransform RelativeTransform = UKismetMathLibrary::MakeRelativeTransform(
			FTransform(FRotator(0.f, ActorPushedTransform.Rotator().Yaw, 0.f), ActorPushedTransform.GetLocation()),  // A.
			GetOwner()->GetActorTransform()                                                                           // Relative To.
		);

		OutRelativeRotation = UKismetMathLibrary::NegateRotator(RelativeTransform.Rotator());

		return UKismetMathLibrary::Vector_Distance(InV1, InV2) / 1.7f;
	}
	return 0.0f;
}


bool UPushingComponent::SearchGroundDistance(float InRadius, float& OutDistanceGround)
{
	if (PushedActor != nullptr)
	{
		// Get pushed actor location.
		const FVector OriginLocation = PushedActor->GetActorLocation();
		FHitResult Result;

		// Create line trace.
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			this,
			OriginLocation,
			OriginLocation - FVector(0.f, 0.f, InRadius * 2.2f),
			PushingTraceType,
			false,
			{ PushedActor },
			IS_DRAW_DEBUG(bDebug),
			Result,
			true
		);

		if (bHit != false)
		{
			OutDistanceGround = OriginLocation.Z - Result.ImpactPoint.Z;
			return true;
		}
	}

	OutDistanceGround = 0.f;
	return false;
}

float UPushingComponent::CalculateGroundAngle(FTwoVectors& OutTargetBoxTrace, float& OutOffset, float InCapsuleOffset)
{
	if (CapsuleComponent != nullptr)
	{
		const float ScaledCapsule = CapsuleComponent->GetScaledCapsuleRadius();
		const FVector OriginLocationOwner = GetOwner()->GetActorLocation();
		const FVector OriginFwdOwner = GetOwner()->GetActorForwardVector();

		OutOffset = ObjectRadius + ScaledCapsule + InCapsuleOffset;
		OutTargetBoxTrace.v1 = OriginLocationOwner;
		OutTargetBoxTrace.v2 = OriginLocationOwner + (OriginFwdOwner * OutOffset);

		return CalculateUpGroundAngle(OriginFwdOwner * OutOffset, 0.5f);
	}
	return 0.0f;
}

FTransform UPushingComponent::SetOriginLocation(const FTwoVectors& InTargetBoxTrace, float InOffset)
{
	FVector HitLocation;

	// Create raycast.
	UNavigationSystemV1::NavigationRaycast(
		this,
		InTargetBoxTrace.v1,
		InTargetBoxTrace.v2,
		HitLocation
	);

	float AlphaLocal = 0.f;
	if (UKismetMathLibrary::NotEqual_VectorVector(HitLocation, FVector(0.f), 1.f))
	{
		const float DistanceV1 = UKismetMathLibrary::Vector_Distance(HitLocation, InTargetBoxTrace.v1);
		const float DistanceV2 = UKismetMathLibrary::Vector_Distance(InTargetBoxTrace.v1, InTargetBoxTrace.v2);

		AlphaLocal = UKismetMathLibrary::MapRangeClamped(
			UKismetMathLibrary::Abs(DistanceV1 - DistanceV2),
			0.f,
			InOffset * 1.f,
			0.2f,
			0.f
		);
	}

	return FTransform(
		FRotator(),
		UKismetMathLibrary::VLerp(InTargetBoxTrace.v2, HitLocation, AlphaLocal)
	);
}

bool UPushingComponent::GetGroundZLocation(const FTransform& InTransformOrigin, FVector& OutGroundLocation)
{
	if ((PushedActor != nullptr) && (CharacterMovement != nullptr))
	{
		FHitResult Result;
		const FVector OriginPushedActorLocation = PushedActor->GetActorLocation();
		const FVector OriginStartLocation = UKismetMathLibrary::VLerp(OriginPushedActorLocation, InTransformOrigin.GetLocation(), 0.5f) -
			FVector(0.f, 0.f, DistanceToGround);

		// Create sphere trace.
		const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
			this,
			OriginStartLocation + FVector(0.f, 0.f, 80.f),
			OriginStartLocation - FVector(0.f, 0.f, 140.f),
			ObjectRadius * 0.3f,
			PushingTraceType,
			false,
			{ PushedActor },
			(bDebug != false) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
			Result,
			true
		);

		if ((bHit != false) && (CharacterMovement->IsWalkable(Result) != false))
		{
			OutGroundLocation = Result.ImpactPoint;
			return true;
		}
	}
	return false;
}

FTransform UPushingComponent::GetFixZLocation(const FTransform& InTransformOrigin, const FVector& OutGroundLocation)
{	
	const FVector OriginVector = InTransformOrigin.GetLocation();

	return FTransform(
		FRotator(),
		FVector(OriginVector.X, OriginVector.Y, (OutGroundLocation.Z + 0.f) + DistanceToGround)
	);
}

FTransform UPushingComponent::SetOriginRotation(const FTransform& InTransformOrigin)
{
	const FRotator OriginRotationOwner = GetOwner()->GetActorRotation();
	
	return FTransform(
		UKismetMathLibrary::NormalizedDeltaRotator(OriginRotationOwner, RelativeRotation),
		InTransformOrigin.GetLocation()
	);
}

FHitResult UPushingComponent::SetActorLocationAndRotation(const FTransform& InputTransform, float DeltaSeconds)
{
	if (PushedActor != nullptr)
	{
		const FVector OriginPushedActorLocation = PushedActor->GetActorLocation();
		
		// Smooth get new location.
		const FVector NewLocation = UKismetMathLibrary::VInterpTo_Constant(
			OriginPushedActorLocation,
			InputTransform.GetLocation(),
			DeltaSeconds,
			500.f
		);
		
		PushedActor->SetActorLocationAndRotation(NewLocation, InputTransform.Rotator(), true);
		PushedMeshComponent->SetPhysicsLinearVelocity(FVector(0.f));
	}
	return FHitResult();
}

void UPushingComponent::SetIKRootRotation()
{
	if (PushedActor != nullptr)
	{
		UArrowComponent* RootIK = PushedActor->GetRootIK();
		const FRotator OriginRot = PushedActor->GetActorRotation();

		// Get delta rotation.
		const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(
			OriginRot,                                 // A
			UKismetMathLibrary::NegateRotator(RelativeRotation)   // B
		);
		
		const FVector VectorLocal = ((ObjectRadius * 0.95f) * 1.f) * UKismetMathLibrary::GetForwardVector(Delta);
		const FRotator RotatorLocal = UKismetMathLibrary::MakeRotFromX(VectorLocal);

		// Set root ik new rotation.
		RootIK->SetWorldRotation(FRotator(OriginRot.Pitch, RotatorLocal.Yaw, OriginRot.Roll));
	}
}

void UPushingComponent::SetIKLocation(float CapsuleOffset)
{
	if (PushedActor != nullptr)
	{
		LeftHandIK = CalculateTranformIK(PushedActor->GetLeftIK());
		RightHandIK = CalculateTranformIK(PushedActor->GetRightIK());
	}
}

FTransform UPushingComponent::CalculateTranformIK(UArrowComponent* InHandIK)
{
	if ((InHandIK != nullptr) && (Character != nullptr))
	{
		USkeletalMeshComponent* Mesh = Character->GetMesh();

		// Get transform component.
		const FVector OriginLocation = InHandIK->GetComponentLocation();
		const FVector OriginFwd = InHandIK->GetForwardVector();
		const FRotator OriginRotation = InHandIK->GetComponentRotation();

		const FVector MakeVector = (OriginLocation + (OriginFwd * (-6.f))) + FVector(0.f, 0.f, -5.f);

		// Return transform.
		return UKismetMathLibrary::MakeRelativeTransform(
				FTransform(OriginRotation, MakeVector),
				Mesh->GetComponentTransform()
		);
	}
	return FTransform();
}

void UPushingComponent::UpdateOwnerRotation(float DeltaSeconds)
{
	if (CharacterMovement != nullptr)
	{
		// Get velocity owner.
		const FVector VelocityLocal = CharacterMovement->Velocity;

		if (VelocityLocal.Size2D() > 50.f)
		{
			const FRotator OriginWorldRotation = GetOwner()->GetActorRotation();
			const FRotator OriginControlRotation = Character->GetControlRotation();

			// Smooth calculate new rotation owner.
			const FRotator NewRotation = UKismetMathLibrary::RInterpTo(
				OriginWorldRotation,
				FRotator(0.f, OriginControlRotation.Yaw, 0.f),
				DeltaSeconds,
				1.f
			);

			// Set new rotation.
			GetOwner()->SetActorRotation(NewRotation);
		}
	}
}