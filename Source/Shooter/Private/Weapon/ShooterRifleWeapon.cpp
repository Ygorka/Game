// Shooter. All Rights Reserved.

#include "Weapon/ShooterRifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/ShooterWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

AShooterRifleWeapon::AShooterRifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<UShooterWeaponFXComponent>("WeaponFXComponent");
}

void AShooterRifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponFXComponent);
}

void AShooterRifleWeapon::StartFire()
{
	InitFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShooterRifleWeapon::MakeShot, TimerBetweenShots, true);
	MakeShot();
}

void AShooterRifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetFXActive(false);
}

void AShooterRifleWeapon::MakeShot()
{
	Super::MakeShot();
	if(!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart;
	FVector TraceEnd;
	if(!GetTraceData(TraceStart,TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	FVector TraceFXEnd = TraceEnd;
	if(HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
	}
	SpawnTraceFX(GetMuzzleWorldLocation(),TraceFXEnd);

	DecreaseAmmo();
}
bool AShooterRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation,ViewRotation)) return false;
	
	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void AShooterRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if(!DamageActor) return;

	DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetController(), this);
}

void AShooterRifleWeapon::InitFX()
{
	if(!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}

	if(!FireAudioComponent)
	{
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
	}
	SetFXActive(true);
}
void AShooterRifleWeapon::SetFXActive(bool IsActive)
{
	if(MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!IsActive);
		MuzzleFXComponent->SetVisibility(IsActive,true);
	}

	if(FireAudioComponent)
	{
		IsActive ? FireAudioComponent->Play() : FireAudioComponent-> Stop();
	}
}

void AShooterRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),TraceFX, TraceStart);
	if(TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

AController* AShooterRifleWeapon::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn -> GetController() : nullptr;
}

