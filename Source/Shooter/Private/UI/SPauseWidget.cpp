// Shooter. All Rights Reserved.


#include "UI/SPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

void USPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &USPauseWidget::OnClearPause);
    }

}

void USPauseWidget::OnClearPause()
{
    if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}