// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterBasePickup.h"
#include "ShooterHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterHealthPickup : public AShooterBasePickup
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float HealthAmount = 100.0f;
private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
