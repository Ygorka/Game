// Shooter. All Rights Reserved.


#include "UI/ShooterPlayerHUDWidget.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterUtils.h"
#include "Components/ProgressBar.h"
#include "Player/SPlayerState.h"

void UShooterPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UShooterPlayerHUDWidget::OneNewPawn);
		OneNewPawn(GetOwningPlayerPawn());
	}
}

void UShooterPlayerHUDWidget::OneNewPawn(APawn* NewPawn)
{
	const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(NewPawn);
	if(HealthComponent && !HealthComponent->OnHealthChange.IsBoundToObject(this))
	{
		HealthComponent->OnHealthChange.AddUObject(this,&UShooterPlayerHUDWidget::OnHealthChanged);
	}

	UpdateHealthBar();
}

void UShooterPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if(HealthDelta < 0.0f)
	{
		OnTakeDamage();
		if(!IsAnimationPlaying(DamageAnimation))
		{
			PlayAnimation(DamageAnimation);
		}
	}

	UpdateHealthBar();
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

int32 UShooterPlayerHUDWidget::GetKillsNum() const
{
	const auto Controller = GetOwningPlayer();
	if(!Controller) return 0;
	const auto PlayerState = Cast<ASPlayerState>(Controller->PlayerState);
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

void UShooterPlayerHUDWidget::UpdateHealthBar()
{
	if(HealthProgressBar)
	{
		HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
	}
}

FString UShooterPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
	const int32 MaxLen = 3;
	const TCHAR PrefixSymbol = '0';

	auto BulletsStr = FString::FromInt(BulletsNum);
	const auto SymbolsNumToAdd = MaxLen - BulletsStr.Len();

	if(SymbolsNumToAdd > 0)
	{
		BulletsStr = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletsStr);
	}

	return BulletsStr;
}