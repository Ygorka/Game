// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterLauncherWeapon.generated.h"

class AShooterProjectile;
UCLASS()
class SHOOTER_API AShooterLauncherWeapon : public AShooterBaseWeapon
{
	GENERATED_BODY()
public:
	virtual void StartFire() override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Wepon")
	TSubclassOf<AShooterProjectile> ProjectileClass;
	
	virtual void MakeShot() override;
	
};
