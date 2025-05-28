// Copyright Vinipi Studios 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "InteractionKeyFactory.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMIZABLEINTERACTIONPLUGINEDITOR_API UInteractionKeyFactory : public UFactory
{
	GENERATED_BODY()

public:
	UInteractionKeyFactory();
	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
};
