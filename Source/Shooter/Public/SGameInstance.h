// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShooterCoreTypes.h"
#include "SGameInstance.generated.h"


UCLASS()
class SHOOTER_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	FLevelData GetStartupLevel() const {return StartupLevel;}
	void SetStartupLevel(const FLevelData& Data) {StartupLevel = Data;}
	
	TArray<FLevelData> GetLevelsData() const {return LevelsData;}
	
	FName GetMenuLevelName() const {return MenuLevelName;}
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TArray<FLevelData> LevelsData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName = NAME_None;

private:
	FLevelData StartupLevel;
};
