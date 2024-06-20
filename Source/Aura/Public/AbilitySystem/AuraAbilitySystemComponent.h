// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;
DECLARE_MULTICAST_DELEGATE_OneParam(FAppliedGameplayEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAuraAbilitySystemComponent();

	void BindOnGameplayEffectApplied();
	void AddAbilitiesToCharacter(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesClass);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	FAppliedGameplayEffectAssetTags AppliedGameplayEffectAssetTags;

protected:
	void OnGameplayEffectApplied(UAbilitySystemComponent* InAbilityComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const; 
};
