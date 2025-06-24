// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SKGShooterFrameworkActors/Public/Actors/SKGFirearm.h"
#include "XYSWeaponActor.generated.h"

class USKGFirearmComponent;
class USKGOffhandIKComponent;

UCLASS(Blueprintable, BlueprintType)
class XYSGAME_API AXYSWeaponActor : public ASKGFirearm
{
	GENERATED_BODY()

public:
	AXYSWeaponActor();

#if UE_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	
protected:
	UPROPERTY(EditDefaultsOnly, Category="XYSWeapon")
	FTransform MeshOffset =  FTransform(FTransform::Identity);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="XYSWeapon")
	TObjectPtr<USceneComponent> SceneComponent;
};
