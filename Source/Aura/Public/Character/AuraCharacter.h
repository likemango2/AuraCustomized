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
	virtual int32 GetCharacterLevel() const override;
	
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes", meta=(AllowPrivateAccess))
	TSubclassOf<UGameplayEffect> InitialPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes", meta=(AllowPrivateAccess))
	TSubclassOf<UGameplayEffect> InitialSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes", meta=(AllowPrivateAccess))
	TSubclassOf<UGameplayEffect> InitialVitalAttributes;
	
	UPROPERTY(EditAnywhere, Category="Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	void GiveStartupAbilities();
	
	// void OnApplyActivateGameplayEffectCallback(UAbilitySystemComponent* SelfAbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;
	// void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& ActiveGameplayEffect) const;
};
