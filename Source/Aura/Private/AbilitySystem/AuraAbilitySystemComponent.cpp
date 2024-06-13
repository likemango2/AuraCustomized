// Copyright XiaoYao.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAuraAbilitySystemComponent::BindOnGameplayEffectApplied()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnGameplayEffectApplied);
}

void UAuraAbilitySystemComponent::OnGameplayEffectApplied(UAbilitySystemComponent* InAbilityComponent,
	const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, FString::Printf(TEXT("Effect Applied : %s"), *GameplayEffectSpec.Def->GetName()));

	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);
	AppliedGameplayEffectAssetTags.Broadcast(TagContainer);
	
	// for (const FGameplayTag& Tag : TagContainer)
	// {
	// 	//TODO: Broadcast the tag to the Widget Controller
	// 	const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
	// 	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, Msg);
	// }
	
}


