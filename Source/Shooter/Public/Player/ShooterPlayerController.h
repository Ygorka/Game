// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterCoreTypes.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

class USRespawnComponent;
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AShooterPlayerController();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USRespawnComponent* RespawnComponent;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
private:
	void OnPauseGame();
	void OnMatchStateChange(ESMatchState State);
};
