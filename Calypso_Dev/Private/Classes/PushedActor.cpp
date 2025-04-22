

#include "Classes/PushedActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Library/HelperLibrary.h"

APushedActor::APushedActor(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh.
	PushedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(PushedMesh);

	// Arrow Root IK.
	HandsIK = CreateDefaultSubobject<UArrowComponent>(TEXT("RootHandsIK"));
	HandsIK->SetupAttachment(PushedMesh);
	HandsIK->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	HandsIK->SetRelativeScale3D(FVector(1.f, 1.f, 1.2f));
	HandsIK->SetArrowSize(0.4f);
	HandsIK->SetArrowColor(FLinearColor::Gray);

	// Arrow Right hand IK.
	RightHandIK = CreateDefaultSubobject<UArrowComponent>(TEXT("RHandsIK"));
	RightHandIK->SetupAttachment(HandsIK);
	RightHandIK->SetRelativeLocation(FVector(-49.f, 27.f, 83.f));
	RightHandIK->SetRelativeScale3D(FVector(0.3125f, 1.f, 1.f));
	RightHandIK->SetArrowSize(0.5f);
	RightHandIK->SetArrowColor(FLinearColor::Blue);

	// Arrow Left hand IK.
	LeftHandIK = CreateDefaultSubobject<UArrowComponent>(TEXT("LHandsIK"));
	LeftHandIK->SetupAttachment(HandsIK);
	LeftHandIK->SetRelativeLocation(FVector(-49.f, -30.f, 83.f));
	LeftHandIK->SetRelativeScale3D(FVector(0.3125f, 1.f, 1.f));
	LeftHandIK->SetArrowSize(0.5f);
	LeftHandIK->SetArrowColor(FLinearColor::Blue);
}

void APushedActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APushedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 APushedActor::FindClosestPushPosition(const FVector& InputActorLocation, float InRange)
{
	// Convert actor 3D location to 2D location.
	const FVector2D ActorLocation2D = UKismetMathLibrary::Conv_VectorToVector2D(InputActorLocation);

	int32 ClosestTransformIndex = -1;
	float ClosestTrasformDistance = 0.f;
	for (int32 i = 0; i < PushTransform.Num(); i++)
	{
		// Get World Location Push Point.
		const FVector TrasformToLocation = UKismetMathLibrary::TransformLocation(GetActorTransform(), PushTransform[i].GetLocation());

		// Calculate square distance between two points.
		const float CurrentDistanceSquare = UKismetMathLibrary::DistanceSquared2D(
			UKismetMathLibrary::Conv_VectorToVector2D(InputActorLocation), 
			UKismetMathLibrary::Conv_VectorToVector2D(TrasformToLocation)
		);

		if (CurrentDistanceSquare < (InRange * InRange))
		{
			if ((CurrentDistanceSquare < ClosestTrasformDistance) || (ClosestTransformIndex < 0))
			{
				ClosestTransformIndex = i;
				ClosestTrasformDistance = CurrentDistanceSquare;
			}
		}
	}

	return ClosestTransformIndex;
}

bool APushedActor::CheckPermissibleFloorAngle(const FVector& InLocation, float InRadius, float InHalfHeight, float InWalkalableFloorZ)
{
	FHitResult Result;
	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		InLocation + FVector(0.f, 0.f, 70.f),
		InLocation - FVector(0.f, 0.f, 100.f),
		InRadius,
		InHalfHeight,
		TraceType,
		false,
		{ this },
		IS_DRAW_DEBUG(bDebug),
		Result,
		true
	);

	if (Result.bStartPenetrating != true)
	{
		return InWalkalableFloorZ < Result.ImpactNormal.Z;
	}
	
	return false;
}

bool APushedActor::CanPushing() const
{
	return CanPushingInternal();
}

bool APushedActor::CanPushingInternal_Implementation() const
{
	return true;
}

bool APushedActor::TryActorStartInteraction_Implementation(AActor* Instigiator)
{
	return false;
}

bool APushedActor::IsCanActorInteraction_Implementation() const
{
	return false;
}

bool APushedActor::IsActorInteraction_Implementation() const
{
	return false;
}

void APushedActor::OnActorBeginDetected_Implementation(AActor* Instigiator)
{
}

void APushedActor::OnActorEndDetected_Implementation(AActor* Instigiator)
{
}

void APushedActor::OnActorBeginInteraction_Implementation(AActor* Instigiator)
{
}

void APushedActor::OnActorEndInteraction_Implementation(AActor* Instigiator)
{
}

FInteractOption APushedActor::GetActorInteractOption_Implementation() const
{
	return FInteractOption();
}