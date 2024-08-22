// Shooter. All Rights Reserved.


#include "UI/SGameOverWidget.h"
#include "ShooterGameModeBase.h"
#include "Player/SPlayerState.h"
#include "UI/SPlayerStatRowWidget.h"
#include "ShooterUtils.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "kismet/GameplayStatics.h"

void USGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if(GetWorld())
    {
        const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
        if(GameMode)
        {
            GameMode->OnMatchStateChange.AddUObject(this, &USGameOverWidget::OnMatchStateChange);
        }
    }
    if(ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &USGameOverWidget::OnResetLevel);
    }
}

void USGameOverWidget::OnMatchStateChange(ESMatchState State)
{
    if(State == ESMatchState::GameOver)
    {
        UpdatePlayerStat();
    }
}

void USGameOverWidget::UpdatePlayerStat()
{
    if(!GetWorld() || !PlayerStatBox) return;

    PlayerStatBox->ClearChildren();

    for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if(!Controller) continue;

        const auto PlayerState = Cast<ASPlayerState>(Controller->PlayerState);
        if(!PlayerState) continue;

        const auto PlayerStatRowWidget = CreateWidget<USPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if(!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(ShooterUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(ShooterUtils::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerStatRowWidget->SetTeam(ShooterUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisible(Controller->IsPlayerController());
        PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());
        PlayerStatBox->AddChild(PlayerStatRowWidget);
    }
}

void USGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}