// Shooter. All Rights Reserved.


#include "Components/SCharacterMovementComponent.h"
#include "Player/ShooterBaseCharacter.h"

float USCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	AShooterBaseCharacter* Player = Cast<AShooterBaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}