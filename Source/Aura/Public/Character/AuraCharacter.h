// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

struct FActiveGameplayEffect;
struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual int32 GetPlayerLevel() const override;
	
private:
	virtual void InitAbilityActorInfo() override;

	// void OnApplyActivateGameplayEffectCallback(UAbilitySystemComponent* SelfAbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;
	// void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& ActiveGameplayEffect) const;
};
