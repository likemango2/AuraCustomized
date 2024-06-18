// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Controller/AuraWidgetController.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
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
	
	UOverlayWidgetController* GetOverlayWidgetController(const FControllerParams& InParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FControllerParams& InParams);
	void CreateOverlayWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, const UAttributeSet* AS);
	
protected:

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<UAuraUserWidget> AttributeMenuWidgetClass;
	// UPROPERTY()
	// TObjectPtr<UAuraUserWidget> AttributeMenuWidget;

public:
	FORCEINLINE UAuraUserWidget* GetOverlayWidget() const { return OverlayWidget; }
	// FORCEINLINE UAuraUserWidget* GetAttributeMenuWidget() const { return AttributeMenuWidget; }
};
