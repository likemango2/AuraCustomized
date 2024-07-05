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
	GiveStartupAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetCharacterLevel() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if(!AuraPlayerState) return Super::GetCharacterLevel();
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
			AuraHUD->CreateOverlayWidget(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void AAuraCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(InitialPrimaryAttributes, GetCharacterLevel());
	ApplyEffectToSelf(InitialSecondaryAttributes, GetCharacterLevel());
	ApplyEffectToSelf(InitialVitalAttributes, GetCharacterLevel());
}
