// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Aura/Aura.h"
#include "Interface/EnemyInterface.h"
#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual int32 GetPlayerLevel() const override;
private:
	UPROPERTY(VisibleAnywhere)
	bool bHighlight = false;

	UPROPERTY()
	int32 Level;

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
		if(bHighlight)
		{
			HighlightActor();
		}
		else
		{
			UnHighlightActor();
		}
	}
	FORCEINLINE bool GetHighlight() const { return bHighlight;}
};
