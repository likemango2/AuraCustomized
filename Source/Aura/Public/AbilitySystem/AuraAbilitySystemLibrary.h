// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* ContextObject);

	UFUNCTION(BlueprintPure)
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* ContextObject);
};
