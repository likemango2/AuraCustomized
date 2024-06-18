// Copyright XiaoYao.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/Controller/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* ContextObject)
{
	if(AAuraPlayerController* PC = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(ContextObject, 0)))
	{
		if(AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>())
		{
			if(AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
			{
				FControllerParams Params;
				Params.AttributeSet = PS->GetAttributeSet();
				Params.AbilitySystemComponent = PS->GetAbilitySystemComponent();
				Params.PlayerController = PC;
				Params.PlayerState = PS;
				return HUD->GetOverlayWidgetController(Params);
			}
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* ContextObject)
{
	if(AAuraPlayerController* PC = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(ContextObject, 0)))
	{
		if(AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>())
		{
			if(AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
			{
				FControllerParams Params;
				Params.AttributeSet = PS->GetAttributeSet();
				Params.AbilitySystemComponent = PS->GetAbilitySystemComponent();
				Params.PlayerController = PC;
				Params.PlayerState = PS;
				return HUD->GetAttributeMenuWidgetController(Params);
			}
		}
	}
	return nullptr;
}
