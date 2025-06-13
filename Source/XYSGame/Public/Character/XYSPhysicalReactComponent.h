// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XYSPhysicalReactComponent.generated.h"


class UPhysicsControlComponent;

UCLASS(meta=(BlueprintSpawnableComponent), Blueprintable)
class XYSGAME_API UXYSPhysicalReactComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UXYSPhysicalReactComponent();

	UFUNCTION(BlueprintCallable)
	void HitReact(bool bRagdoll, const FVector& HitLocation, const FName& HitBoneName);
	
protected:
	
	//1.初始化PhysicsControl
	void SetupPhysicsControl();

	//2.为物理受击反馈做准备
	void SetupPhysicsHit();

	


private:
	UPROPERTY()
	TObjectPtr<UPhysicsControlComponent> PhysicsControlComponent;
};
