// XiaoYao copyright.


#include "Weapon/XYSWeaponActor.h"

AXYSWeaponActor::AXYSWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetNetUpdateFrequency(100);
}

// #if WITH_EDITOR
// void AXYSWeaponActor::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 	if (PropertyChangedEvent.Property)
// 	{
// 		const FName PropertyName = PropertyChangedEvent.Property->GetFName();
//
// 		if (PropertyName == GET_MEMBER_NAME_CHECKED(AXYSWeaponActor, MeshOffset))
// 		{
// 			FirearmMeshComponent->SetRelativeTransform(MeshOffset);
// 		}
// 	}	
// }
// #endif

