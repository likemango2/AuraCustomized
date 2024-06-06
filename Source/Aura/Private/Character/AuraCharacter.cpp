// Copyright XiaoYao.


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


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
