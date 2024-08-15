// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	virtual void OnPossess(APawn* InPawn) override;
};
