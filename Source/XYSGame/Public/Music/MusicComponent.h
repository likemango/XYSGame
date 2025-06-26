// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MusicComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class XYSGAME_API UMusicComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ReceiveWeaponFire();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ReceivePlayerDeath();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void FireStinger();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetIsMenu(bool InBool);
};
