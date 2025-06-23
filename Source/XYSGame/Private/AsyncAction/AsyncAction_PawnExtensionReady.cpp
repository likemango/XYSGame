// XiaoYao copyright.


#include "AsyncAction/AsyncAction_PawnExtensionReady.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "XYSGameplayTags.h"
#include "XYSLogChannels.h"
#include "AbilitySystem/XYSAbilitySystemComponent.h"
#include "Character/XYSPawnExtensionComponent.h"

UAsyncAction_PawnExtensionReady* UAsyncAction_PawnExtensionReady::CreateUAsyncActionPawnExtensionReady(APawn* ContextPawn)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(ContextPawn, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PawnExtensionReady* Node = NewObject<UAsyncAction_PawnExtensionReady>();
			Node->CachedPawn = ContextPawn;
			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}
	return nullptr;
}

void UAsyncAction_PawnExtensionReady::Activate()
{
	if (CachedPawn.IsValid() && CachedPawn.Get()->HasAuthority())
	{
		OnPawnExtensionReady_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnCachedPawnExtensionReady));
	}
}

void UAsyncAction_PawnExtensionReady::OnCachedPawnExtensionReady() const
{
	UE_LOG(LogXYSAbilitySystem, Warning, TEXT("UAsyncAction_PawnExtensionReady called!!!"))
	
	// UXYSAbilitySystemComponent* AbilitySystemComponent = Cast<UXYSAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CachedPawn.Get()));
	// check(AbilitySystemComponent);
	OnPawnExtensionReady.Broadcast();
}

void UAsyncAction_PawnExtensionReady::OnPawnExtensionReady_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate) const
{
	if (UXYSPawnExtensionComponent* PawnExtensionComponent = UXYSPawnExtensionComponent::FindPawnExtensionComponent(CachedPawn.Get()))
	{
		if (!PawnExtensionComponent->OnPawnExtensionReady.IsBoundToObject(Delegate.GetUObject()))
		{
			PawnExtensionComponent->OnPawnExtensionReady.Add(Delegate);
		}
		if (PawnExtensionComponent->GetInitState() == XYSGameplayTags::InitState_GameplayReady)
		{
			Delegate.Execute();
		}
	}
}
