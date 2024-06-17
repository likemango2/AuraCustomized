// Copyright XiaoYao.


#include "UI/Controller/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
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

	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChangeCallback);
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChangeCallback);
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChangeCallback);
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChangeCallback);

	Cast<UAuraAbilitySystemComponent>(Params.AbilitySystemComponent)->AppliedGameplayEffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// const FString Msg = FString::Printf(TEXT("GE Tag: %s"0), *Tag.ToString());
				// GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
				FUIWidgetRow* Row = GetDatatableRow<FUIWidgetRow>(MessageWidgetDatatable, Tag);
				MessageWidgetRowSignature.Broadcast(*Row);
			}
		}
	);
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
