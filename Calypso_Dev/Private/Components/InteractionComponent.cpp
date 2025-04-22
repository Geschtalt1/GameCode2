

#include "Components/InteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Library/HelperLibrary.h"

#include "Interface/InteractableSystemInterface.h"

#include "Logging/LogMacros.h"

UInteractionComponent::UInteractionComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	StartSearchInteraction();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Try draw debug for interaction actor.
	if (bDebug != false) { DrawDebugForInteractionActor(DeltaTime); }
}

void UInteractionComponent::StartSearchInteraction()
{
	if (IsSearchActive() != true)
	{
		UE_LOG(LogInteraction, Display, TEXT("Start Search Interaction!"));

		// Start timer.
		GetWorld()->GetTimerManager().SetTimer(
			SearchTimer, 
			this, 
			&UInteractionComponent::OnFindInteractiveActors, 
			TimerRate, 
			true
		);

		// Call Delegate.
		OnActorInteractionUpdateEnabled.Broadcast(GetOwner(), true);
	}
}

void UInteractionComponent::StopSearchInteraction(bool bClearDetectedActors)
{
	if (IsSearchActive() != false)
	{
		UE_LOG(LogInteraction, Display, TEXT("Stop Search Interaction!"));

		// Clear Timer.
		GetWorld()->GetTimerManager().ClearTimer(SearchTimer);

		if (bClearDetectedActors != false)
		{
			// Clear current detected actors.
			ClearActors(DetectedInteractables);
		}

		// Call Delegate.
		OnActorInteractionUpdateEnabled.Broadcast(GetOwner(), false);
	}
}

void UInteractionComponent::OnFindInteractiveActors()
{
	TArray<FHitResult> Results;
	if (CreateSphereTrace(Results) != false)
	{
		TArray<AActor*> ActorsLocal = {};
		TArray<AActor*> DetectedActors = {};
		const FVector OriginLocation = GetOwner()->GetActorLocation();
		
		// Filter found actors.
		FilterInteractiveActors(Results, ActorsLocal, true);
		UHelperLibrary::FindNearestActors(OriginLocation, ActorsLocal, DetectedActors, SearchInteractiveInRadius, SearchTolerance, MaxDetectedActors);
		SetActorsDetected(DetectedActors); // <- REPLACE.

		// Check enabled debug.
		if ((bool)bDebug != false)
		{
			DrawDebugForDetectedActors(TimerRate);
		}
	}
	else
	{
		ClearActors(DetectedInteractables);
	}

	float Distance;
	AActor* ActorLocal = UGameplayStatics::FindNearestActor(
		GetOwner()->GetActorLocation(),
		DetectedInteractables,
		Distance
	);
}

void UInteractionComponent::OnActorFinishInteraction()
{
	UE_LOG(LogInteraction, Display, TEXT("Finish interaction: %s"), *CurrentInteractionActor->GetName());

	StopCurrentInteraction();
}

bool UInteractionComponent::CreateSphereTrace(TArray<FHitResult>& Results)
{
	return UKismetSystemLibrary::SphereTraceMulti(
		this,
		GetOwner()->GetActorLocation() + OffsetTrace,
		GetOwner()->GetActorLocation() + OffsetTrace,
		SearchInteractiveInRadius,
		InteractionTraceType,
		false,
		{GetOwner()},
		IS_DRAW_DEBUG(bDebug),
		Results,
		true,
		FLinearColor::Red,
		FLinearColor::Blue,
		TimerRate
	);
}

bool UInteractionComponent::FilterInteractiveActors(const TArray<FHitResult>& InputResult, TArray<AActor*>& OutActors, bool bCheckCanInterac) const
{
	for (const FHitResult& Hit : InputResult)
	{
		AActor* OverlappingActor = Hit.GetActor();
		
		// Check if there is an interface interactable in the actor.
		if (IsActorInteractive(OverlappingActor) != false)
		{
			if (bCheckCanInterac != false)
			{
				if (CheckCanActorInteract(OverlappingActor) != true)
				{
					continue;
				}
			}

			OutActors.Add(OverlappingActor);
		}
	}
	return OutActors.Num() > 0;
}

bool UInteractionComponent::CheckCanActorInteract(AActor*& CheckActor) const
{
	return IInteractableSystemInterface::Execute_IsCanActorInteraction(CheckActor);
}

void UInteractionComponent::SetActorsDetected(const TArray<AActor*>& NewActors)
{
	TArray<AActor*> OldDetectedActors;
	FindOldDetectedActors(NewActors, OldDetectedActors);
	ClearActors(OldDetectedActors);

	if (NewActors.Num() > 0)
	{
		for (const auto& ActorLocal : NewActors)
		{
			TryAddActorDetected(ActorLocal);
		}
	}
}

bool UInteractionComponent::TryAddActorDetected(AActor* InActorDetected)
{
	// Check if actor interaction.
	if ((IsActorInteractive(InActorDetected) != false) && (DetectedInteractables.Contains(InActorDetected) != true))
	{
		DetectedInteractables.Add(InActorDetected);
		
		// Call delegate & interface in actor.
		IInteractableSystemInterface::Execute_OnActorBeginDetected(InActorDetected, GetOwner());
		OnActorInteractionUpdateDetected.Broadcast(GetOwner(), FIteractionDetected(true, InActorDetected));

		UE_LOG(LogInteraction, Display, TEXT("Added new actor detected: %s"), *InActorDetected->GetName());

		return true;
	}
	else
	{
		if (InActorDetected != nullptr)
		{
			UE_LOG(LogInteraction, Warning, TEXT("Falied add actor detected: %s"), *InActorDetected->GetName());
		}
		else
		{
			UE_LOG(LogInteraction, Warning, TEXT("Falied add actor detected: nullptr"));
		}
	}
	return false;
}

bool UInteractionComponent::RemoveActorDetected(AActor* InActorDetected)
{
	if (DetectedInteractables.Contains(InActorDetected) != false)
	{
		UE_LOG(LogInteraction, Display, TEXT("Remove Actor Detected: %s"), *InActorDetected->GetName());

		// Remove actor from array.
		DetectedInteractables.Remove(InActorDetected);

		// Call delegate & interface in actor.
		IInteractableSystemInterface::Execute_OnActorEndDetected(InActorDetected, GetOwner());
		OnActorInteractionUpdateDetected.Broadcast(GetOwner(), FIteractionDetected(false, InActorDetected));
		
		return true;
	}
	return false;
}

void UInteractionComponent::FindOldDetectedActors(const TArray<AActor*>& InputActors, TArray<AActor*>& OutActors)
{
	for (const auto& ActorLocal : DetectedInteractables)
	{
		// Find Old Actors.
		if (InputActors.Contains(ActorLocal) != true)
		{
			OutActors.AddUnique(ActorLocal);
		}
	}

	UE_LOG(LogInteraction, Display, TEXT("Find Old Detected Actors: %d"), OutActors.Num());
}

void UInteractionComponent::ClearActors(const TArray<AActor*>& InputActors)
{
	if (InputActors.Num() > 0)
	{
		// Remove all actors from array.
		for (const auto& ActorLocal : InputActors)
		{
			RemoveActorDetected(ActorLocal);
		}

		UE_LOG(LogInteraction, Display, TEXT("Clear All Interaction Actors!"));
	}
}

void UInteractionComponent::DrawDebugForDetectedActors(float InDuration)
{
	if (DetectedInteractables.Num() > 0)
	{
		for (const auto& ActorLocal : DetectedInteractables)
		{
			if (CurrentInteractionActor == ActorLocal)
			{
				continue;
			}

			// Get Bounds Actor.
			FVector Center, Extern;
			ActorLocal->GetActorBounds(true, Center, Extern);

			// Draw Debug Box For Actors.
			UKismetSystemLibrary::DrawDebugBox(
				this,
				Center,
				Extern,
				FLinearColor(0.f, 0.f, 0.75f),
				FRotator(),
				InDuration,
				1.5f
			);
		}
	}
}

void UInteractionComponent::DrawDebugForInteractionActor(float InDuration)
{
	if (CurrentInteractionActor != nullptr)
	{
		// Get Bounds Actor.
		FVector Center, Extern;
		CurrentInteractionActor->GetActorBounds(true, Center, Extern);

		// Draw Debug Box For Actors.
		UKismetSystemLibrary::DrawDebugBox(
			this,
			Center,
			Extern,
			FLinearColor::Green,
			FRotator(),
			InDuration,
			2.5f
		);
	}
}

bool UInteractionComponent::IsActorInteractive(const AActor* CheckActor) const
{
	return (CheckActor != nullptr) ? (CheckActor->GetClass()->ImplementsInterface(UInteractableSystemInterface::StaticClass())) : (false);
}

bool UInteractionComponent::TryStartInteraction()
{
	/** Replace logic. */
	
	float Distance;
	AActor* ActorLocal = UGameplayStatics::FindNearestActor(
		GetOwner()->GetActorLocation(),
		DetectedInteractables,
		Distance
	);

	return SetActorInteraction(ActorLocal);
}

bool UInteractionComponent::SetActorInteraction(AActor* NewActor)
{
	if (NewActor != nullptr)
	{
		// Clear old actor interaction.
		StopCurrentInteraction();

		if (auto InteractInterface = Cast<IInteractableSystemInterface>(NewActor))
		{
			// Try call interface.
			if (IInteractableSystemInterface::Execute_TryActorStartInteraction(NewActor, GetOwner()))
			{
				CurrentInteractionActor = NewActor;

				// Bind delegate & call delegate start interact.
				InteractInterface->OnReceiveActorEndInteraction.AddDynamic(this, &UInteractionComponent::OnActorFinishInteraction);
				IInteractableSystemInterface::Execute_OnActorBeginInteraction(CurrentInteractionActor, GetOwner());
				FInteractOption Option = IInteractableSystemInterface::Execute_GetActorInteractOption(CurrentInteractionActor);
			
				OnActorStartInteraction.Broadcast(GetOwner(), FInteractAction(CurrentInteractionActor, Option));
				
				UE_LOG(LogInteraction, Display, TEXT("Set Actor Interaction: %s"), *CurrentInteractionActor->GetName());

				// Stop interacty if interact type = instance.
				if (Option.InteractType == EInteractType::IT_Instance)
				{
					StopCurrentInteraction();
				}
				
				return true;
			}
		}
		else
		{
			UE_LOG(LogInteraction, Warning, TEXT("Failed Set Actor Interaction: %s"), *NewActor->GetName());
		}
	}
	return false;
}

void UInteractionComponent::StopCurrentInteraction()
{
	if (CurrentInteractionActor != nullptr)
	{
		UE_LOG(LogInteraction, Display, TEXT("Stop interaction for: %s"), *CurrentInteractionActor->GetName());

		if (auto InteractInterface = Cast<IInteractableSystemInterface>(CurrentInteractionActor))
		{
			// Remove bind delegate.
			InteractInterface->OnReceiveActorEndInteraction.Remove(this, "OnActorFinishInteraction");
	
			// Call interfcae end iteraction in the current interaction actor.
			IInteractableSystemInterface::Execute_OnActorEndInteraction(CurrentInteractionActor, GetOwner());
			FInteractOption Option = IInteractableSystemInterface::Execute_GetActorInteractOption(CurrentInteractionActor);

			// Call delegate end interaction.
			OnActorEndInteraction.Broadcast(GetOwner(), FInteractAction(CurrentInteractionActor, Option));

		}

		// Clear pointer.
		CurrentInteractionActor = nullptr;
	}
}