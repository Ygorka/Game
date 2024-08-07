// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SNeedAmmoDecorator.generated.h"

class AShooterBaseWeapon;

UCLASS()
class SHOOTER_API USNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	USNeedAmmoDecorator();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<AShooterBaseWeapon> WeaponType;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
