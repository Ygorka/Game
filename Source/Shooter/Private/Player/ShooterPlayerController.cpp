// Shooter. All Rights Reserved.


#include "Player/ShooterPlayerController.h"
#include "Components/SRespawnComponent.h"
#include "ShooterGameModeBase.h"

AShooterPlayerController::AShooterPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USRespawnComponent>("RespawnComponent");
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetWorld())
	{
		const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameMode)
		{
			GameMode->OnMatchStateChange.AddUObject(this, &AShooterPlayerController::OnMatchStateChange);
		}
	}
}

void AShooterPlayerController::OnMatchStateChange(ESMatchState State)
{
	if(State == ESMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());  
		bShowMouseCursor = true;
	}
}

void AShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn);
}

void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(!InputComponent) return;

	InputComponent->BindAction("Pause", IE_Pressed,this, &AShooterPlayerController::OnPauseGame);
}

void AShooterPlayerController::OnPauseGame()
{
	if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->SetPause(this);
}