// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

struct FControllerParams;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class UAuraWidgetController;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAuraHUD();
	
	UOverlayWidgetController* GetController(const FControllerParams& InParams);
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, const UAttributeSet* AS);
	
protected:

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> ControllerClass;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> Controller;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

public:
	FORCEINLINE UAuraUserWidget* GetOverlayWidget() const { return OverlayWidget; }
	
};
