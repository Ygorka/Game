// Shooter. All Rights Reserved.


#include "AI/ShooterAICharacter.h"
#include "AI/ShooterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/SHealthBarWidget.h"
#include "Components/ShooterHealthComponent.h"

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

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void AShooterAICharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
}

void AShooterAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHealthWidgetVisibility();
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

void AShooterAICharacter::OnHealthChange(float Health, float HealthDelta)
{
	Super::OnHealthChange(Health, HealthDelta);

	const auto HealthBarWidget = Cast<USHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if(!HealthBarWidget) return;
	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void AShooterAICharacter::UpdateHealthWidgetVisibility()
{
	if(!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) return;
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());

	HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}