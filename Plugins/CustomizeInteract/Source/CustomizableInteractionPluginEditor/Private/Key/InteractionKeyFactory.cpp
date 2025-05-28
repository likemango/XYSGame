// Copyright Vinipi Studios 2024. All rights reserved.

#include "Key/InteractionKeyFactory.h"
#include "Core/InteractionKey.h"

UInteractionKeyFactory::UInteractionKeyFactory()
{
	SupportedClass = UInteractionKey::StaticClass();
	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}

UObject* UInteractionKeyFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UInteractionKey>(InParent, InClass, InName, Flags);
}

bool UInteractionKeyFactory::CanCreateNew() const
{
	return true;
}
