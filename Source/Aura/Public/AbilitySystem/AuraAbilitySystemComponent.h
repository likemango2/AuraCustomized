// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAppliedGameplayEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAuraAbilitySystemComponent();

	void BindOnGameplayEffectApplied();

	FAppliedGameplayEffectAssetTags AppliedGameplayEffectAssetTags;

protected:
	void OnGameplayEffectApplied(UAbilitySystemComponent* InAbilityComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const; 
};
