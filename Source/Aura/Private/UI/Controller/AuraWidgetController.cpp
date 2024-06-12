// Copyright XiaoYao.


#include "UI/Controller/AuraWidgetController.h"

UAuraWidgetController::UAuraWidgetController()
{
	
}

void UAuraWidgetController::SetControllerParams(const FControllerParams& InParams)
{
	Params.AttributeSet = InParams.AttributeSet;
	Params.PlayerController = InParams.PlayerController;
	Params.PlayerState = InParams.PlayerState;
	Params.AbilitySystemComponent = InParams.AbilitySystemComponent;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	
}
