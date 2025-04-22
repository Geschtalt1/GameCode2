
#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "Classes/InteractiveActor.h"
#include "Interface/ItemInterface.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class CALYPSO_DEV_API AItemBase : public AInteractiveActor, public IItemInterface
{
	GENERATED_BODY()
	
public:
	AItemBase(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

public:
	virtual UItemData* GetItemData_Implementation() const;

protected:
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "On Loaded Item Data"), Category = Item)
	void OnLoadedItemDataInternal(UItemData* Data);
	virtual void OnLoadedItemDataInternal_Implementation(UItemData* Data);

	/*
	 * Кастомное событие. Вызывается когда мешь предмета был загружен. 
	 * Обработайте событие для установки меша в StaticMeshComponent или SkeletalMeshComponent.
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "On Loaded Mesh"), Category = Item)
	void OnLoadedMeshInternal(UObject* Mesh);
	virtual void OnLoadedMeshInternal_Implementation(UObject* Mesh);

	/*
	 * Кастомное событие. Установите мешь предмета, для загрузки.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = Item)
	TSoftObjectPtr<UObject> GetLoadMesh() const;
	virtual TSoftObjectPtr<UObject> GetLoadMesh_Implementation() const;

protected:
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;

private:
	/** Проверяет */
	void LoadItemData();

	/** Загружает мешь предмета. */
	void LoadMesh();

private:
	UFUNCTION()
	void OnLoadedMesh();

private:
	/** Item Data. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Item)
	TObjectPtr<UItemData> ItemData = nullptr;

private:
	FStreamableManager StreamableManager;
	TSharedPtr<FStreamableHandle> Handle;
};
