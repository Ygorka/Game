// Shooter. All Rights Reserved.


#include "UI/ShooterGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "ShooterGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameHUD, All, All);

void AShooterGameHUD::DrawHUD()
{
	Super::DrawHUD();
	
	//DrawCrossHair();
}

void AShooterGameHUD::BeginPlay()
{
	Super::BeginPlay();

	GameWidgets.Add(ESMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(ESMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(ESMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

	for(auto GameWidgetPair : GameWidgets)
	{
		const auto GameWidget = GameWidgetPair.Value;
		if(!GameWidget) continue;

		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if(GetWorld())
	{
		const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameMode)
		{
			GameMode->OnMatchStateChange.AddUObject(this, &AShooterGameHUD::OnMatchStateChange);
		}
	}
}

void AShooterGameHUD::OnMatchStateChange(ESMatchState State)
{
	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if(GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets[State];
	}

	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	}
	UE_LOG(LogGameHUD, Display, TEXT("MatchState is : %s"), *UEnum::GetValueAsString(State));
}

void AShooterGameHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LinearColor = FLinearColor::Green;
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LinearColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LinearColor, LineThickness);
}

