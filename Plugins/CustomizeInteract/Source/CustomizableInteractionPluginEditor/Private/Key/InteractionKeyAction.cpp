// Copyright Vinipi Studios 2024. All rights reserved.

#include "Key/InteractionKeyAction.h"

#include "Core/InteractionKey.h"

UInteractionKeyAction::UInteractionKeyAction(EAssetTypeCategories::Type Category)
{
	AssetCategory = Category;
}

FText UInteractionKeyAction::GetName() const
{
	return FText::FromString("Interaction Key");
}

FColor UInteractionKeyAction::GetTypeColor() const
{
	return FColor(255, 111, 0);
}

UClass* UInteractionKeyAction::GetSupportedClass() const
{
	return UInteractionKey::StaticClass();
}

void UInteractionKeyAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}

uint32 UInteractionKeyAction::GetCategories()
{
	return AssetCategory;
}
