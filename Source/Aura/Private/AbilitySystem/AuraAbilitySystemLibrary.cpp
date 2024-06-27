// Copyright XiaoYao.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameMode/AuraGameModeBase.h"
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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContext, ECharacterClass CharacterClass,
	float Level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraAM = CastChecked<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	check(AuraAM->CharacterClassInfo);

	AActor* AvatarActor = ASC->GetAvatarActor();
	
	FCharacterClassDefaultInfo ClassDefaultInfo = AuraAM->CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesEffectContext = ASC->MakeEffectContext();
	PrimaryAttributesEffectContext.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle PrimaryGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryGameplayEffectSpecHandle.Data);

	FGameplayEffectContextHandle SecondaryAttributesEffectContext = ASC->MakeEffectContext();
	SecondaryAttributesEffectContext.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle SecondaryGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(AuraAM->CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryGameplayEffectSpecHandle.Data);

	FGameplayEffectContextHandle VitalAttributesEffectContext = ASC->MakeEffectContext();
	VitalAttributesEffectContext.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle VitalGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(AuraAM->CharacterClassInfo->VitalAttributes, Level, VitalAttributesEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalGameplayEffectSpecHandle.Data);
}
