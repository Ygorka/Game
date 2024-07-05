// Shooter. All Rights Reserved.


#include "Weapon/ShooterBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Viewports.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

AShooterBaseWeapon::AShooterBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void AShooterBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
}

void AShooterBaseWeapon::Fire()
{
	UE_LOG(LogBaseWeapon, Display, TEXT("FIRE"));
	MakeShoot();
}

void AShooterBaseWeapon::MakeShoot()
{
	if(!GetWorld()) return;

	const auto Player = Cast<ACharacter>(GetOwner());
	
	if(!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if(!Controller) return;

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	const FVector TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);

	if(HitResult.bBlockingHit)
	{
		MakeDamage(HitResult, Controller);
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}
}

void AShooterBaseWeapon::MakeDamage(const FHitResult& HitResult, APlayerController* Controller)
{
	const auto DamageActor = HitResult.GetActor();
	if(!DamageActor) return;

	DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, Controller, this);
}


