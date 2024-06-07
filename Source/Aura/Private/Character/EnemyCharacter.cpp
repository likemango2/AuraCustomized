// Copyright XiaoYao.


#include "Character/EnemyCharacter.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyCharacter::HighlightActor()
{
	bHighlight = true;
}

void AEnemyCharacter::UnHighlightActor()
{
	bHighlight = false;
}

