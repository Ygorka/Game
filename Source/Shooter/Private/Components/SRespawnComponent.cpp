// Shooter. All Rights Reserved.


#include "Components/SRespawnComponent.h"
#include "ShooterGameModeBase.h"

USRespawnComponent::USRespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USRespawnComponent::Respawn(int32 RespawnTime)
{
	if(!GetWorld()) return;

	RespawnCountDown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USRespawnComponent::RespawnTimerUpdate, 1.0f, true);
	
}

void USRespawnComponent::RespawnTimerUpdate()
{
	if(--RespawnCountDown == 0)
	{
		if(!GetWorld()) return;
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
		if(!GameMode) return;
		GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	}
}

bool USRespawnComponent::IsRespawnInProgress() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}