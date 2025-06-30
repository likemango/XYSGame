// XiaoYao copyright.

#pragma once


#include "XYSNumberPopComponent.h"

#include "XYSNumberPopComponent_NiagaraText.generated.h"

class UXYSDamagePopStyleNiagara;
class UNiagaraComponent;
class UObject;

UCLASS(Blueprintable)
class UXYSNumberPopComponent_NiagaraText : public UXYSNumberPopComponent
{
	GENERATED_BODY()

public:

	UXYSNumberPopComponent_NiagaraText(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UXYSNumberPopComponent interface
	virtual void AddNumberPop(const FXYSNumberPopRequest& NewRequest) override;
	//~End of UXYSNumberPopComponent interface

protected:
	
	TArray<int32> DamageNumberArray;

	/** Style patterns to attempt to apply to the incoming number pops */
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UXYSDamagePopStyleNiagara> Style;

	//Niagara Component used to display the damage
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UNiagaraComponent> NiagaraComp;
};
