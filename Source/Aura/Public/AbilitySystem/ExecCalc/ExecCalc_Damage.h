// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	FGameplayEffectAttributeCaptureDefinition BlockChanceDef;
	FGameplayEffectAttributeCaptureDefinition ArmorDef;
	FGameplayEffectAttributeCaptureDefinition ArmorPenetrationDef;
	FGameplayEffectAttributeCaptureDefinition CriticalHitChanceDef;
	FGameplayEffectAttributeCaptureDefinition CriticalHitResistanceDef;
	FGameplayEffectAttributeCaptureDefinition CriticalHitDamageDef;
	FGameplayEffectAttributeCaptureDefinition FireResistanceDef;
	FGameplayEffectAttributeCaptureDefinition LightningResistanceDef;
	FGameplayEffectAttributeCaptureDefinition ArcaneResistanceDef;
	FGameplayEffectAttributeCaptureDefinition PhysicalResistanceDef;

	UPROPERTY()
	TMap<FName, FGameplayEffectAttributeCaptureDefinition> ResistanceToCaptureDefinition;

};
