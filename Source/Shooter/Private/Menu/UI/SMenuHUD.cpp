// Shooter. All Rights Reserved.


#include "Menu/UI/SMenuHUD.h"
#include "Blueprint/UserWidget.h"

void ASMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if(MenuWidgetClass)
    {
        const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if(MenuWidget)
        {
            MenuWidget->AddToViewport();
        }
    }
}
