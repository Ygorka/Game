// Shooter. All Rights Reserved.


#include "UI/SGameDataWidget.h"
#include "ShooterGameModeBase.h"
#include "Player/SPlayerState.h"

int32 USGameDataWidget::GetKillsNum() const
{
	const auto PlayerState = GetSPlayerState();
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 USGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetSGameMode();
	return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 USGameDataWidget::GetTotalRoundsNum() const
{
	const auto GameMode = GetSGameMode();
	return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 USGameDataWidget::GetRoundsRemaining() const
{
	const auto GameMode = GetSGameMode();
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

AShooterGameModeBase* USGameDataWidget::GetSGameMode() const
{
	return GetWorld() ? Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASPlayerState* USGameDataWidget::GetSPlayerState() const
{
	return GetOwningPlayer() ? Cast<ASPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}