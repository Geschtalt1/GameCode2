

#include "Classes/ItemBase.h"
#include "Data/ItemData.h"

#include "Kismet/KismetSystemLibrary.h"

AItemBase::AItemBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	LoadItemData();
}

void AItemBase::PostActorCreated()
{
	Super::PostActorCreated();
}

void AItemBase::LoadItemData()
{
	if (ItemData != nullptr)
	{
		OnLoadedItemDataInternal(ItemData);
		LoadMesh();
	}
}

void AItemBase::LoadMesh()
{
	auto ItemMeshLocal = GetLoadMesh();
	if (UKismetSystemLibrary::IsValidSoftObjectReference(ItemMeshLocal) != false)
	{
		// Get path to static or skeletal mesh item.
		FSoftObjectPath PathToMesh = ItemMeshLocal.ToSoftObjectPath();
		
		// Start load mesh.
		Handle = StreamableManager.RequestAsyncLoad(
			PathToMesh,
			FStreamableDelegate::CreateUObject(this, &AItemBase::OnLoadedMesh)
		);
	}
}

void AItemBase::OnLoadedMesh()
{
	if ((Handle.IsValid() != false) && (Handle->HasLoadCompleted() != false))
	{
		OnLoadedMeshInternal(Handle->GetLoadedAsset());
	}
}

void AItemBase::OnLoadedMeshInternal_Implementation(UObject* Mesh)
{
	///...
}

void AItemBase::OnLoadedItemDataInternal_Implementation(UItemData* Data)
{
	///...
}

TSoftObjectPtr<UObject> AItemBase::GetLoadMesh_Implementation() const
{
	return TSoftObjectPtr<UObject>();
}

UItemData* AItemBase::GetItemData_Implementation() const
{
	return ItemData;
}

#if WITH_EDITOR 
    void AItemBase::OnConstruction(const FTransform& Transform)
    {
		Super::OnConstruction(Transform);
    }
#endif