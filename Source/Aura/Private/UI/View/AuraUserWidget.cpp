// Copyright XiaoYao.


#include "UI/View/AuraUserWidget.h"

void UAuraUserWidget::SetController(UObject* InController)
{
	Controller = InController;
	OnControllerSet();
}
