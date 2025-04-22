
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

USTRUCT(BlueprintType)
struct FIteractionDetected
{
	GENERATED_BODY()

public:
	FIteractionDetected()
	{}

	FIteractionDetected(bool InSuccessfully, AActor* InTarget)
		: bSuccessfullySensed(InSuccessfully),
		  Target(InTarget)
	{}

public:
	/** Возращает true, если актор является обнаруженным и false если он перестал был обнаруженным. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bSuccessfullySensed : 1;

	/** Ссылка на замеченый актор. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> Target;
};

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	/** Сквозное действие. */
	IT_Instance                    UMETA(DisplayName = "Instance"),

	/** Продолжительное действие. */
	IT_Duration                    UMETA(DisplayName = "Duration")
};

USTRUCT(BlueprintType)
struct FInteractOption
{
	GENERATED_BODY()

public:
	FInteractOption()
	{}

public:
	/** Название актора. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ActorDisplayName;

	/** Название действия. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ActionDisplayName;

	/** Тип взаимодействия. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractType InteractType;
};

USTRUCT(BlueprintType)
struct FInteractAction
{
	GENERATED_BODY()

public:
	FInteractAction()
	{}

	FInteractAction(AActor* InTarget, FInteractOption InOption)
		: Target(InTarget),
		  Option(InOption)
	{}

public:
	/** Ссылка на актор с которым происходит взаимодействие. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> Target;

	/** Параметры взаимодействия. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInteractOption Option;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateInteractionEnabled, class AActor*, Actor, bool, bEnabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateIteractionDetectedActor, class AActor*, Actor, FIteractionDetected, Detected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateInteractionAction, class AActor*, Actor, FInteractAction, Action);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateInteractionStatus, class AActor*, Actor);

DEFINE_LOG_CATEGORY_STATIC(LogInteraction, All, All);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CALYPSO_DEV_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent(const FObjectInitializer& ObjectInitializer);

public:
	/** Запускает поиск интерактивных объектов. */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Search")
	void StartSearchInteraction();

	/** Останавливает поиск интерактивных объектов. */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Search")
	void StopSearchInteraction(bool bClearDetectedActors = true);

	/** Пробует запустить интеракцию с предметом. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool TryStartInteraction();

	/** Отфильтровывает только интерактивных акторов и возращает их массивом. */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Filter")
	bool FilterInteractiveActors(const TArray<FHitResult>& InputResult, TArray<AActor*>& OutActors, bool bCheckCanInterac = false) const;

	/** Устанавливает массив новых обнаруженных акторов. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetActorsDetected(const TArray<AActor*>& NewActors);

	/** Устанавливает актора взаимодействия. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool SetActorInteraction(AActor* NewActor);

	/** Останавливает текущего взаимодействие с актором если оно есть. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void StopCurrentInteraction();

	/** Пытается добавить актора в массив, если он является интерактивным. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool TryAddActorDetected(AActor* InActorDetected);

	/** Удаляяет обнаруженного актора из массива. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool RemoveActorDetected(AActor* InActorDetected);

	/** Дебаг функция. Отображает дебаг на заммеченных объектов. */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Debug")
	void DrawDebugForDetectedActors(float InDuration = 2.f);

	/** Дебаг функция. Отображает дебаг на взаимодествуещем акторе. */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Debug")
	void DrawDebugForInteractionActor(float InDuration = 2.f);

public:
	/** Возращает true, если поиск интерактивных предметов активен. */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Condition")
	FORCEINLINE bool IsSearchActive() const { return (GetWorld() != nullptr) ? (GetWorld()->GetTimerManager().IsTimerActive(SearchTimer)) : (false); }

	/** Возращает true, если актор интерактивный. */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Condition")
	FORCEINLINE bool IsActorInteractive(const AActor* CheckActor) const;

	/** Возращает актора, над котором происходит взаимодействие. **/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FORCEINLINE AActor* GetCurrentInteractionActor() const { return CurrentInteractionActor; }

	/** Возращает true, если в текущий момент владельц производит взаимодействие с объектом. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FORCEINLINE bool IsInteractionActive() const { return CurrentInteractionActor != nullptr; }

public:
	/** Делегат обновления статуса компонента. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Interaction Update Enabled"))
	FOnUpdateInteractionEnabled OnActorInteractionUpdateEnabled;

	/** Делегат обновления замеченых акторов. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Interaction Update Detected"))
	FOnUpdateIteractionDetectedActor OnActorInteractionUpdateDetected;

	/** Делегат начало взаимодействия с актором. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor Start Interaction"))
	FOnUpdateInteractionAction OnActorStartInteraction;

	/** Делегат окончания взаимодействия с актором. */
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Actor End Interaction"))
	FOnUpdateInteractionAction OnActorEndInteraction;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** Вызывается для поиска интерактивных объектов в радиусе. */
	UFUNCTION()
	void OnFindInteractiveActors();

	/** Вызывается когда актор завершил взаимодествие. */
	UFUNCTION()
	void OnActorFinishInteraction();

private:
	/** Создает трейс. */
	FORCEINLINE bool CreateSphereTrace(TArray<FHitResult>& Results);

	/** Находит старых обнаруженных акторов, которых уже нет в радиусе обнаружения. */
	FORCEINLINE void FindOldDetectedActors(const TArray<AActor*>& InputActors, TArray<AActor*>& OutActors);

	/** Удаляет список акторов из массива заммеченных акторов. */
	FORCEINLINE void ClearActors(const TArray<AActor*>& InputActors);

	/** Проверяет может ли актор быть использован. */
	FORCEINLINE bool CheckCanActorInteract(AActor*& CheckActor) const;

private:
	/** Радиус в котором обнаруживаются интерактивные объекты. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "50"), Category = "Interaction|Search")
	float SearchInteractiveInRadius = 100.f;

	/** Допуск радиуса в котором обнаруживаются объекты. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0"), Category = "Interaction|Search")
	float SearchTolerance = 30.f;

	/** Кол-во максимальных обнаруживаемых объектов. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0"), Category = "Interaction|Search")
	int32 MaxDetectedActors = 5;

	/** Скорость таймера обнаружения интерактивных объектов. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "1"), Category = "Interaction|Search")
	float TimerRate = 0.16677f;

	/** Тип интерактивного трейса. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Interaction")
	TEnumAsByte<ETraceTypeQuery> InteractionTraceType;

	/** Смещение трейса. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Interaction")
	FVector OffsetTrace;

	/** Дебаг трассировки поиска интерактивных акторов. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Interaction")
	uint8 bDebug:1 = false;

	/** Таймер поиска. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Interaction")
	FTimerHandle SearchTimer;

	/** Список обнаруженых интерактивных объектов в указаном радиусе. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Interaction")
	TArray<TObjectPtr<AActor>> DetectedInteractables;

	/** Указатель на текущего актора взаимодествия. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Interaction")
	TObjectPtr<AActor> CurrentInteractionActor;
};
