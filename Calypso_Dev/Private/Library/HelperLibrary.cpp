

#include "Library/HelperLibrary.h"

#include "GameFramework/Character.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "CollisionQueryParams.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"

float UHelperLibrary::GetActorAngleFloor(const AActor* InputActor, FVector OffsetTrace, ECollisionChannel Channel)
{
	if (InputActor != nullptr)
	{
		// Определяем параметры лайн трейса.
		FHitResult HitResult;
		FVector Start = InputActor->GetActorLocation() + OffsetTrace;
		FVector End = InputActor->GetActorLocation() + UKismetMathLibrary::NegateVector(OffsetTrace);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InputActor);

		const UWorld* WorldLocal = InputActor->GetWorld();

		// Создаем лайн трейс.
		const bool bHit = WorldLocal->LineTraceSingleByChannel(HitResult, Start, End, Channel, QueryParams);


#if ENABLE_DRAW_DEBUG
		DrawDebugLine(WorldLocal, Start, End, bHit ? FColor::Red : FColor::Green, false, 0.1f, 0, 1.0f);
#endif

		if (bHit)
		{
			FVector Normal = HitResult.Normal;
			Normal.Normalize();
			FVector UpVector = FVector(0.0f, 0.0f, 1.0f);

			float DotProduct = FVector::DotProduct(Normal, UpVector);
			float AngleRadians = FMath::Acos(DotProduct);
			float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

			return AngleDegrees;
		}
	}

	return 0.0f;
}

EPhysicalSurface UHelperLibrary::GetActorSurfaceFloor(AActor* InputActor, float ZOffset, ETraceTypeQuery Channel)
{
	if (InputActor != nullptr)
	{
		FHitResult HitResult;
		const FRotator RotationActor = InputActor->GetActorRotation();
		
		// Расчитываем конец вектора, относительно вращения персонажа.
		FVector Up = (UKismetMathLibrary::GetUpVector(RotationActor) * ZOffset) * (-1.0f);
		FVector Start = InputActor->GetActorLocation();
		FVector End = Start + Up;
		
		// Параметры трейса.
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InputActor);

		// Создаем лайн трейс.
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			InputActor,
			Start,
			End,
			Channel,
			false,
			{ InputActor },
			EDrawDebugTrace::None,
			HitResult,
			true
		);

#if ENABLE_DRAW_DEBUG
		DrawDebugLine(InputActor, Start, End, bHit ? FColor::Red : FColor::Green, false, 0.1f, 0, 1.0f);
#endif

		// Возращаем тип поверхности.
		return UGameplayStatics::GetSurfaceType(HitResult);
	}
	return EPhysicalSurface();
}

FVector UHelperLibrary::NormalToVector(FVector Normal)
{
	FRotator n = UKismetMathLibrary::MakeRotFromX(Normal);
	n = FRotator(0, n.Yaw - 180, 0);

	return UKismetMathLibrary::GetForwardVector(n);
}

bool UHelperLibrary::FindNearestActors(const FVector& Origin, const TArray<AActor*>& InActors, TArray<AActor*>& OutActors, float Radius, float Tolerance, int32 MaxResult)
{
	if ((InActors.Num() > 0) && (MaxResult > 0))
	{
		// Фильтруем акторов по радиусу и создаем структуру, содержащую актора и расстояние до него.
		TArray<TPair<float, AActor*>> DistanceActorPairs;
		for (const auto& ActorLocal : InActors)
		{
			const float Distance = FVector::Distance(Origin, ActorLocal->GetActorLocation());
			if (Distance <= (Radius + Tolerance))
			{
				DistanceActorPairs.Add(TPair<float, AActor*>(Distance, ActorLocal));
			}
		}

		// Сортируем акторов по расстоянию (от ближайшего к дальнему).
		Algo::Sort(DistanceActorPairs, [](const TPair<float, AActor*>& A, const TPair<float, AActor*>& B) {
			return A.Key < B.Key; // Сортировка по расстоянию (ключу).
			});

		// Заполняем выходной массив.
		for (const auto& Pair : DistanceActorPairs)
		{
			OutActors.Add(Pair.Value);

			// Проверяем что достигли лимит выходных акторов.
			if (OutActors.Num() == MaxResult)
			{
				break;
			}
		}
	}
	return OutActors.Num() > 0;
}