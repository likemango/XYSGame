// Copyright Vinipi Studios 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * Used to define the interaction key for the engine.
 */
class CUSTOMIZABLEINTERACTIONPLUGINEDITOR_API UInteractionKeyAction : public FAssetTypeActions_Base
{
	
public:
	UInteractionKeyAction(EAssetTypeCategories::Type Category);
	
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type AssetCategory;
};
