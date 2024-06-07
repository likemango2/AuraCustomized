// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Aura/Aura.h"
#include "Interface/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class AURA_API AEnemyCharacter : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

private:
	UPROPERTY(VisibleAnywhere)
	bool bHighlight = false;

public:
	FORCEINLINE void SetHighlight(bool bCond)
	{
		if(bCond)
		{
			UE_LOG(LogAura, Warning, TEXT("%s is highlight"), *GetName())
		}
		else
		{
			UE_LOG(LogAura, Warning, TEXT("%s is Unhighlight"), *GetName())
		}
		bHighlight = bCond;
	}
	FORCEINLINE bool GetHighlight() const { return bHighlight;}
};
