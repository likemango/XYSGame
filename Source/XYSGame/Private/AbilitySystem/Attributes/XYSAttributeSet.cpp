// XiaoYao copyright.


#include "AbilitySystem/Attributes/XYSAttributeSet.h"

#include "AbilitySystem/XYSAbilitySystemComponent.h"

UXYSAttributeSet::UXYSAttributeSet()
{
}

UWorld* UXYSAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UXYSAbilitySystemComponent* UXYSAttributeSet::GetXYSAbilitySystemComponent() const
{
	return Cast<UXYSAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
