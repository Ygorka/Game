// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ShooterWeaponComponent.h"
#include "SAIWeaponComponent.generated.h"

UCLASS()
class SHOOTER_API USAIWeaponComponent : public UShooterWeaponComponent
{
	GENERATED_BODY()
public:
	virtual void StartFire() override;
	virtual void NextWeapon() override;
};
