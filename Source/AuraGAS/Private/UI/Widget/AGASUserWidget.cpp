// Copyright Dustin Carmelo


#include "UI/Widget/AGASUserWidget.h"

void UAGASUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
