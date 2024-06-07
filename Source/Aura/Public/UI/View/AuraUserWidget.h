// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetController(UObject* InController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> Controller;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnControllerSet();
};
