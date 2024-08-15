// Shooter. All Rights Reserved.


#include "Player/SPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerState, All, All);

void ASPlayerState::LogInfo()
{
	UE_LOG(LogPlayerState, Display, TEXT("TeamID: %i, kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}