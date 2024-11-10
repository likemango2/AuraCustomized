// Copyright XiaoYao.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interface/CombatInterface.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	ArmorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ArmorDef.AttributeToCapture = UAuraAttributeSet::GetArmorAttribute();
	ArmorDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(ArmorDef);

	BlockChanceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockChanceDef.AttributeToCapture = UAuraAttributeSet::GetBlockChanceAttribute();
	BlockChanceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(BlockChanceDef);

	CriticalHitChanceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CriticalHitChanceDef.AttributeToCapture = UAuraAttributeSet::GetCriticalHitChanceAttribute();
	CriticalHitChanceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(CriticalHitChanceDef);

	CriticalHitDamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CriticalHitDamageDef.AttributeToCapture = UAuraAttributeSet::GetCriticalHitDamageAttribute();
	CriticalHitDamageDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(CriticalHitDamageDef);

	CriticalHitResistanceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CriticalHitResistanceDef.AttributeToCapture = UAuraAttributeSet::GetCriticalHitResistanceAttribute();
	CriticalHitResistanceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(CriticalHitResistanceDef);

	FireResistanceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	FireResistanceDef.AttributeToCapture = UAuraAttributeSet::GetFireResistanceAttribute();
	FireResistanceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(FireResistanceDef);
	
	LightningResistanceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	LightningResistanceDef.AttributeToCapture = UAuraAttributeSet::GetLightningResistanceAttribute();
	LightningResistanceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(LightningResistanceDef);
	
	ArcaneResistanceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ArcaneResistanceDef.AttributeToCapture = UAuraAttributeSet::GetArcaneResistanceAttribute();
	ArcaneResistanceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(ArcaneResistanceDef);
	
	PhysicalResistanceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	PhysicalResistanceDef.AttributeToCapture = UAuraAttributeSet::GetPhysicalResistanceAttribute();
	PhysicalResistanceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(PhysicalResistanceDef);

	ResistanceToCaptureDefinition.Add(FName("Attributes.Resistance.Fire"), FireResistanceDef);
	ResistanceToCaptureDefinition.Add(FName("Attributes.Resistance.Lightning"), LightningResistanceDef);
	ResistanceToCaptureDefinition.Add(FName("Attributes.Resistance.Arcane"), ArcaneResistanceDef);
	ResistanceToCaptureDefinition.Add(FName("Attributes.Resistance.Physical"), PhysicalResistanceDef);

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

	AActor* SourceAvatar = ExecutionParams.GetSourceAbilitySystemComponent() ? ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = ExecutionParams.GetTargetAbilitySystemComponent() ? ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	// get damage by SetByCallerMagnitude and take resistance into consideration.
	float Damage = 0;
	for(const TTuple<FGameplayTag, FGameplayTag>& Tuple : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Tuple.Key;
		const FGameplayTag ResistanceTypeTag = Tuple.Value;

		checkf(ResistanceToCaptureDefinition.Contains(ResistanceTypeTag.GetTagName()), TEXT("ResistanceToCaptureDefinition doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTypeTag.ToString());
		float ResistanceValue = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceToCaptureDefinition[ResistanceTypeTag.GetTagName()], EvaluationParameters, ResistanceValue);
		ResistanceValue = FMathf::Clamp(ResistanceValue, 0, 100.f);
		
		float TypeDamage = ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(DamageTypeTag, false);
		TypeDamage = FMathf::Max(0, TypeDamage);
		
		TypeDamage *= (100.f - ResistanceValue) / 100.f;
		Damage += TypeDamage;
	}

	// target blockChance
	float TargetBlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);
	bool bBlocked = FMath::RandRange(0.f, 100.f) < TargetBlockChance;
	Damage = bBlocked ? Damage / 2.0f : Damage;
	
	float TargetArmor = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	float SourceArmorPenetration = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName(TEXT("ArmorPenetration")), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetCharacterLevel());

	// Source armor penetration ignores a percentage of the target's armor
	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName(TEXT("EffectiveArmor")), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetCharacterLevel());
	// Armor ignores a percentage of incoming damage 
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
	
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetCharacterLevel());

	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	// Double damage plus a bonus if critical hit
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

	FGameplayEffectContextHandle EffectContextHandle = ExecutionParams.GetOwningSpec().GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	const FGameplayModifierEvaluatedData ModifierEvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(ModifierEvaluatedData);
}
