// Shooter. All Rights Reserved.


#include "Components/ShooterAIPerceptionComponent.h"
#include "AIController.h"
#include "ShooterUtils.h"
#include "ShooterHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* UShooterAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
	if(PerceiveActors.Num() == 0) return nullptr;

	const auto Controller = Cast<AAIController>(GetOwner());
	if(!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if(!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;

	for(const auto PerceiveActor : PerceiveActors)
	{
		const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(PerceiveActor);
		if(HealthComponent && !HealthComponent->IsDead())
		{
			const auto CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if(CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PerceiveActor;
			}
		}
	}

	return BestPawn;
}