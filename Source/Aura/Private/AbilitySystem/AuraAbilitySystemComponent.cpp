// Copyright XiaoYao.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAuraAbilitySystemComponent::BindOnGameplayEffectApplied()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientOnGameplayEffectApplied);
}

void UAuraAbilitySystemComponent::AddAbilitiesToCharacter(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesClass)
{
	for(const TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesClass)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		// GiveAbility(GameplayAbilitySpec);
		// GiveAbilityAndActivateOnce(GameplayAbilitySpec);
		if(const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientOnGameplayEffectApplied_Implementation(
	UAbilitySystemComponent* InAbilityComponent, const FGameplayEffectSpec& GameplayEffectSpec,
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, FString::Printf(TEXT("Effect Applied : %s"), *GameplayEffectSpec.Def->GetName()));

	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);
	AppliedGameplayEffectAssetTags.Broadcast(TagContainer);
}
