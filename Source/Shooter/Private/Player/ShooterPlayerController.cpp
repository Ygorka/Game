// Shooter. All Rights Reserved.


#include "Player/ShooterPlayerController.h"
#include "Components/SRespawnComponent.h"

AShooterPlayerController::AShooterPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USRespawnComponent>("RespawnComponent");
}

void AShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn);
}