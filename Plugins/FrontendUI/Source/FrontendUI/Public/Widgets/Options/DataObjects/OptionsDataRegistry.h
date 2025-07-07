// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObject_Base;
class UListDataObject_Collection;
/**
 * 
 */
UCLASS()
class FRONTENDUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	//Gets called by options screen right after the object of type UOptionsDataRegistry is createdAdd commentMore actions
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections;}
	TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& TabID);
	
private:
	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer);

	void FindChildListDataRecursively(const UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const;

	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;
};
