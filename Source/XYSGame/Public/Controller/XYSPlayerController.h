// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "XYSPlayerController.generated.h"

class AXYSPlayerState;
class UXYSAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class XYSGAME_API AXYSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AXYSPlayerController(const FObjectInitializer& ObjectInitializer);
	
	//~APlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End

	UFUNCTION(BlueprintCallable, Category = "XYS|PlayerController")
	AXYSPlayerState* GetXYSPlayerState() const;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	UXYSAbilitySystemComponent* GetXYSAbilitySystemComponent() const;
	
};
