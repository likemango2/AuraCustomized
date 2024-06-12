// Copyright XiaoYao.


#include "UI/HUD/AuraHUD.h"

#include "UI/Controller/OverlayWidgetController.h"
#include "UI/View/AuraUserWidget.h"

AAuraHUD::AAuraHUD()
{

}

UOverlayWidgetController* AAuraHUD::GetController(const FControllerParams& InParams)
{
	if(!Controller)
	{
		Controller = NewObject<UOverlayWidgetController>(this, ControllerClass);
		Controller->SetControllerParams(InParams);
	}
	return Controller;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, const UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(ControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	const FControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	Controller = GetController(WidgetControllerParams);
	
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->AddToViewport();

	OverlayWidget->SetController(Controller);
}
