// Shooter. All Rights Reserved.


#include "AI/Services/SChangeWeaponService.h"
#include "Components/ShooterWeaponComponent.h"
#include "AIController.h"
#include "ShooterUtils.h"

USChangeWeaponService::USChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void USChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();

	if(!Controller)
	{
		const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(Controller->GetPawn());
		if(WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
		{
			WeaponComponent->NextWeapon();
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}