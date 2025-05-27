#include "MotionComponentFunctionLibrary.h"
#include "MotionCore/Character/MotionCharacterMovementComponent.h"

bool UMotionComponentFunctionLibrary::OverlapBlockingTestByChannel(UMotionCharacterMovementComponent* MovementComponent, const FVector &Pos, const float CollisionShapeHeight)
{
	return MovementComponent->OverlapBlockingTestByChannel(Pos, CollisionShapeHeight);
}

FString UMotionComponentFunctionLibrary::GetIdentifier(const UObject* Object)
{
	return Object->GetClass()->GetName();
}
