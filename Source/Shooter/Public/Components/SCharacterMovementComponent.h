// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API USCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movment", meta = (ClampMin="1.5", ClampMax="20.0"))
	float RunModifier = 2.0;
	virtual float GetMaxSpeed() const override;
	
};
