// Copyright XiaoYao.


#include "UI/Controller/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(Params.AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(Params.AttributeSet);

	FOnGameplayAttributeValueChange& OnGameplayAttributeHealthValueChange = Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute());
	OnGameplayAttributeHealthValueChange.AddUObject(this, &UOverlayWidgetController::HealthChangeCallback);

	FOnGameplayAttributeValueChange& OnGameplayAttributeMaxHealthValueChange = Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute());
	OnGameplayAttributeMaxHealthValueChange.AddUObject(this, &UOverlayWidgetController::HealthChangeCallback);

	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChangeCallback);
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChangeCallback);
}

void UOverlayWidgetController::HealthChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	OnHealthChanged.Broadcast(OnAttributeChangeData.NewValue);
}

void UOverlayWidgetController::MaxHealthChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	OnMaxHealthChanged.Broadcast(OnAttributeChangeData.NewValue);
}

void UOverlayWidgetController::ManaChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	OnManaChanged.Broadcast(OnAttributeChangeData.NewValue);
}

void UOverlayWidgetController::MaxManaChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	OnMaxManaChanged.Broadcast(OnAttributeChangeData.NewValue);
}
