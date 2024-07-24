// Shooter. All Rights Reserved.


#include "Components/ShooterHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);
// Sets default values for this component's properties
UShooterHealthComponent::UShooterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UShooterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);
	
	AActor* ComponentOwner = GetOwner();
	if(ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this,&UShooterHealthComponent::OnTakeAnyDamage);
	}
	
}

void UShooterHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.0f || IsDead() || !GetWorld()) return;
	
	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	
	if(IsDead())
	{
		OnDeath.Broadcast();
	}
	else if(AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle,this,&UShooterHealthComponent::HealUpdate, HealUpdateTime,true, HealDelay);
	}
	
	PlayCameraShake();
}

void UShooterHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if(IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UShooterHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth,0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;
	
	Health = NextHealth;
	OnHealthChange.Broadcast(Health, HealthDelta);
}

bool UShooterHealthComponent::TryToAddHealth(float HealthAmount)
{
	if(IsDead() || IsHealthFull())
	{
		return false;
	}

	SetHealth(Health + HealthAmount);
	return true;
}
bool UShooterHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}
void UShooterHealthComponent::PlayCameraShake()
{
	if(IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if(!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if(!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
