// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomizableInteractionPluginEditor.h"

#include "Core/InteractionKey.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "Key/InteractionKeyAction.h"

IMPLEMENT_MODULE(FCustomizableInteractionPluginEditorModule, CustomizableInteractionPluginEditor)void FCustomizableInteractionPluginEditorModule::StartupModule()
{
	IAssetTools& AssetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type AssetType = AssetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("InteractionPlugin")), FText::FromString("Customizable Interaction Plugin"));
	TSharedPtr<UInteractionKeyAction> InteractionKeyAction = MakeShareable(new UInteractionKeyAction(AssetType));
	AssetToolsModule.RegisterAssetTypeActions(InteractionKeyAction.ToSharedRef());
}

void FCustomizableInteractionPluginEditorModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
