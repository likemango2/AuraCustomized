// Copyright XiaoYao.


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AuraAbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

const UAttributeSet* AAuraPlayerState::GetAttributeSet() const
{
	if(GetAbilitySystemComponent())
	{
		return GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass());
	}
	return nullptr;
}
