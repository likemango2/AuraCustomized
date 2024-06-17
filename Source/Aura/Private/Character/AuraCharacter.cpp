// Copyright XiaoYao.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 当Controller改变朝向，仅在移动过程中，将角色朝向 朝向 移动方向
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 只要Controller改变朝向，那么自动用RotationRate拟合朝向
	// uint8 bUseControllerDesiredRotation:1;

	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// server
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if(!AuraPlayerState) return Super::GetPlayerLevel();
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->BindOnGameplayEffectApplied();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
	// AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &AAuraCharacter::OnApplyActivateGameplayEffectCallback);
	// AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &AAuraCharacter::OnRemoveGameplayEffectCallback);
}

// void AAuraCharacter::OnApplyActivateGameplayEffectCallback(UAbilitySystemComponent* SelfAbilitySystemComponent,const FGameplayEffectSpec& GameplayEffectSpec,
// 	FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
// {
// 	UE_LOG(LogAura, Warning, TEXT("Apply Gameplay Effect: %s" ), *GameplayEffectSpec.Def->GetName());
// 	UE_LOG(LogAura, Warning, TEXT("It's in the FActiveGameplayEffectsContainer index: %s" ), *ActiveGameplayEffectHandle.ToString());
// }
//
// void AAuraCharacter::OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& ActiveGameplayEffect) const
// {
// 	UE_LOG(LogAura, Warning, TEXT("Remove Gameplay Effect: %s" ), *ActiveGameplayEffect.Spec.Def->GetName());
// }
