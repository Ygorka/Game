// Shooter. All Rights Reserved.


#include "AI/ShooterAIController.h"
#include "AI/ShooterAICharacter.h"
#include "Components/ShooterAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AShooterAIController::AShooterAIController()
{
	ShooterAIPerceptionComponent = CreateDefaultSubobject<UShooterAIPerceptionComponent>("ShooterPerceptionComponent");
	SetPerceptionComponent(*ShooterAIPerceptionComponent);
}
void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto ShooterCharacter = Cast<AShooterAICharacter>(InPawn);
	if(ShooterCharacter)
	{
		RunBehaviorTree(ShooterCharacter->BehaviorTreeAsset);
	}
}
 void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}
AActor* AShooterAIController::GetFocusOnActor() const
{
	if(!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}