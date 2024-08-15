// Shooter. All Rights Reserved.


#include "UI/ShooterPlayerHUDWidget.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterUtils.h"

bool UShooterPlayerHUDWidget::Initialize()
{
	if(GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UShooterPlayerHUDWidget::OneNewPawn);
		OneNewPawn(GetOwningPlayerPawn());
	}
	return Super::Initialize();
}

void UShooterPlayerHUDWidget::OneNewPawn(APawn* NewPawn)
{
	const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(NewPawn);
	if(HealthComponent && !HealthComponent->OnHealthChange.IsBoundToObject(this))
	{
		HealthComponent->OnHealthChange.AddUObject(this,&UShooterPlayerHUDWidget::OnHealthChanged);
	}
}

void UShooterPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if(HealthDelta < 0.0f)
	{
		OnTakeDamage();
	}
}
float UShooterPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(GetOwningPlayerPawn());
	
	if(!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool UShooterPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(GetOwningPlayerPawn());
	
	if(!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UShooterPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	
	const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(GetOwningPlayerPawn());
	
	if(!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UShooterPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool UShooterPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}
