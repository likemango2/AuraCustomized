// Copyright XiaoYao.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
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
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContext);
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesEffectContext = ASC->MakeEffectContext();
	PrimaryAttributesEffectContext.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle PrimaryGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryGameplayEffectSpecHandle.Data);

	FGameplayEffectContextHandle SecondaryAttributesEffectContext = ASC->MakeEffectContext();
	SecondaryAttributesEffectContext.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle SecondaryGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryGameplayEffectSpecHandle.Data);

	FGameplayEffectContextHandle VitalAttributesEffectContext = ASC->MakeEffectContext();
	VitalAttributesEffectContext.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle VitalGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalGameplayEffectSpecHandle.Data);
}

void UAuraAbilitySystemLibrary::InitializeCommonAbilities(const UObject* WorldContext, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContext);
	for(const TSubclassOf<UGameplayAbility>& Ability : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(Ability, Level);
		AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if(!AuraGM) return nullptr;
	return AuraGM->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}
