// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameDataWidget.generated.h"

class AShooterGameModeBase;
class ASPlayerState;

UCLASS()
class SHOOTER_API USGameDataWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetKillsNum() const;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCurrentRoundNum() const;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetTotalRoundsNum() const;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetRoundsRemaining() const;

private:
	AShooterGameModeBase* GetSGameMode() const;
	ASPlayerState* GetSPlayerState() const;
};
