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
	void SetHighlight(bool bCond);
	FORCEINLINE bool GetHighlight() const { return bHighlight;}
};
