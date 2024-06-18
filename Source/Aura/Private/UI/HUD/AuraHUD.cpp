// Copyright XiaoYao.


#include "UI/HUD/AuraHUD.h"

#include "UI/Controller/AttributeMenuWidgetController.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "UI/View/AuraUserWidget.h"

AAuraHUD::AAuraHUD()
{

}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FControllerParams& InParams)
{
	if(!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetControllerParams(InParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FControllerParams& InParams)
{
	if(!AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetControllerParams(InParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::CreateOverlayWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, const UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget OverlayWidgetController Class uninitialized, please fill out BP_AuraHUD"));

	const FControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	
	OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetController(OverlayWidgetController);
	OverlayWidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}
