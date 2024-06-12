// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FControllerParams
{
	GENERATED_BODY()

	FControllerParams() {}
	FControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, const UAttributeSet* AS)
	{
		PlayerController = PC;
		PlayerState = PS;
		AbilitySystemComponent = ASC;
		AttributeSet = AS;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const UAttributeSet> AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UAuraWidgetController();

	UFUNCTION(BlueprintCallable)	
	void SetControllerParams(const FControllerParams& InParams);
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
protected:

#pragma region Model-Layer
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	FControllerParams Params;
#pragma endregion
	
};
