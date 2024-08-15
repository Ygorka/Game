// Shooter. All Rights Reserved.


#include "UI/SSpectatorWidget.h"
#include "ShooterUtils.h"
#include "Components/SRespawnComponent.h"

bool USSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = ShooterUtils::GetShooterPlayerComponent<USRespawnComponent>(GetOwningPlayer());
	if(!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}