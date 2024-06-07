// Copyright XiaoYao.


#include "UI/HUD/AuraHUD.h"

#include "UI/View/AuraUserWidget.h"

AAuraHUD::AAuraHUD()
{

}

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
	
	check(OverlayWidgetClass);

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->AddToViewport();
	
}
