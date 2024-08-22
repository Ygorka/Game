// Shooter. All Rights Reserved.


#include "UI/SGoToMenurWidget.h"
#include "Components/Button.h"
#include "SGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogGoToMenuWidget, All, All)
void USGoToMenurWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(GoToMenuButton)
    {
        GoToMenuButton->OnClicked.AddDynamic(this, &USGoToMenurWidget::OnGoToMenu);
    }
}

void USGoToMenurWidget::OnGoToMenu()
{
    if(!GetWorld()) return;
    
    const auto SGameInstance = GetWorld()->GetGameInstance<USGameInstance>();
    if(!SGameInstance) return;

    if(SGameInstance->GetMenuLevelName().IsNone())
    {
        UE_LOG(LogGoToMenuWidget, Error, TEXT("Menu level name is none"));
        return;
    }
    UGameplayStatics::OpenLevel(this, SGameInstance->GetMenuLevelName());
}