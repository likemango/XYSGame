// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PawnExtensionReady.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAsyncActionOnPawnExtensionReadyDelegate);

/**
 * 
 */
UCLASS()
class XYSGAME_API UAsyncAction_PawnExtensionReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true"), DisplayName="Wait For PawnExtension Ready")
	static UAsyncAction_PawnExtensionReady* CreateUAsyncActionPawnExtensionReady(APawn* ContextPawn);

	UPROPERTY(BlueprintAssignable)
	FAsyncActionOnPawnExtensionReadyDelegate OnPawnExtensionReady;

	virtual void Activate() override;

	UFUNCTION()
	void OnCachedPawnExtensionReady() const;
	
private:
	void OnPawnExtensionReady_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate) const;

	UPROPERTY()
	TWeakObjectPtr<APawn> CachedPawn;
};
