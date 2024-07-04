// Shooter. All Rights Reserved.


#include "Weapon/ShooterBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

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
	
}

void AShooterBaseWeapon::Fire()
{
	UE_LOG(LogBaseWeapon, Display, TEXT("FIRE"));
}

