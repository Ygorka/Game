// Shooter. All Rights Reserved.


#include "AI/ShooterAICharacter.h"
#include "AI/ShooterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SAIWeaponComponent.h"
#include "BrainComponent.h"

AShooterAICharacter::AShooterAICharacter(const FObjectInitializer& ObjInit)
:Super(ObjInit.SetDefaultSubobjectClass<USAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = AShooterAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}

void AShooterAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto SController = Cast<AAIController>(Controller);
	if(SController && SController->BrainComponent)
	{
		SController->BrainComponent->Cleanup();
	}
}
