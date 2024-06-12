// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"


class UGameplayEffect;

UCLASS()
class AURA_API AEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
};
