// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura/Aura.h"
#include "Interface/EnemyInterface.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UBehaviorTree;
class AAuraAIController;
enum class ECharacterClass : uint8;
class UWidgetComponent;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual int32 GetCharacterLevel() const override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void InitializeStartupAbilities() const;
	virtual void Die() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LiftSpan = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
	
private:
	UPROPERTY(VisibleAnywhere)
	bool bHighlight = false;

	UPROPERTY()
	int32 Level;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBar;

public:
	void SetHighlight(bool bCond);
	FORCEINLINE bool GetHighlight() const { return bHighlight;}
};
