// Copyright XiaoYao.


#include "UI/Controller/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	check(AttributeInfo);

	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(Params.AttributeSet);
	for (const TTuple<FGameplayTag, FGameplayAttribute>& Pair : AS->TagToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(Params.AttributeSet);
	UAuraAbilitySystemComponent* ASC = CastChecked<UAuraAbilitySystemComponent>(Params.AbilitySystemComponent);
	for (const TTuple<FGameplayTag, FGameplayAttribute>& Pair : AS->TagToAttributes)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda([this, Pair](const FOnAttributeChangeData& OnAttributeChangeData)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value);
		});
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(Params.AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
