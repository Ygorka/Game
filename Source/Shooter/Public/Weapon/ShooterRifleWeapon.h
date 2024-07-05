// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterRifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterRifleWeapon : public AShooterBaseWeapon
{
	GENERATED_BODY()
public:
	virtual void StartFire() override;
	virtual void StopFire() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Damage")
	float TimerBetweenShots = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Damage")
	float BulletSpread = 1.5f;
	
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
private:
	FTimerHandle ShotTimerHandle;
};
