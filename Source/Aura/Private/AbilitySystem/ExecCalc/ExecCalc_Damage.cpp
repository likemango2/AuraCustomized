// Copyright XiaoYao.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystem/AuraAttributeSet.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	ArmorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ArmorDef.AttributeToCapture = UAuraAttributeSet::GetArmorAttribute();
	ArmorDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(ArmorDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Armor = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(0.f, Armor);
	
	const FGameplayModifierEvaluatedData ModifierEvaluatedData(UAuraAttributeSet::GetArmorAttribute(), EGameplayModOp::Additive, 10);
	OutExecutionOutput.AddOutputModifier(ModifierEvaluatedData);
}
