// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterRifleWeapon.generated.h"

class UShooterWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class SHOOTER_API AShooterRifleWeapon : public AShooterBaseWeapon
{
	GENERATED_BODY()
public:
	AShooterRifleWeapon();
	virtual void StartFire() override;
	virtual void StopFire() override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float TimerBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float BulletSpread = 1.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	FString TraceTargetName = "TraceTarget";
	
	UPROPERTY(VisibleAnywhere, Category="VFX")
	UShooterWeaponFXComponent* WeaponFXComponent;

	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
private:
	
	FTimerHandle ShotTimerHandle;

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;

	UPROPERTY()
	UAudioComponent* FireAudioComponent;
	
	void MakeDamage(const FHitResult& HitResult);
	void InitFX();
	void SetFXActive(bool IsActive);
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

	AController* GetController() const;
};
