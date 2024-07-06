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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float TimerBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float BulletSpread = 1.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float DamageAmount = 10.0f;
	
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
private:
	FTimerHandle ShotTimerHandle;
	void MakeDamage(const FHitResult& HitResult);
};
