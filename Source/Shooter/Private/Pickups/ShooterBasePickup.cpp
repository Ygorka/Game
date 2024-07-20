// Shooter. All Rights Reserved.


#include "Pickups/ShooterBasePickup.h"

#include "Animation/AnimInstanceProxy.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

AShooterBasePickup::AShooterBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);

}

void AShooterBasePickup::BeginPlay()
{
	Super::BeginPlay();

	check(CollisionComponent);
	GenerateRotationYaw();
	
}

void AShooterBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));

}

void AShooterBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
		
	const auto Pawn = Cast<APawn>(OtherActor);
	if(GivePickupTo(Pawn))
	{
		PickupWasTaken();
	}
}

bool AShooterBasePickup::GivePickupTo(APawn* PlayerPawn)
{
	return false;
}

void AShooterBasePickup::PickupWasTaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if(GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
	}
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AShooterBasePickup::Respawn, RespawnTime);
}
void AShooterBasePickup::Respawn()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	if(GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
	GenerateRotationYaw();
}

void AShooterBasePickup::GenerateRotationYaw()
{
	const auto Direction = FMath::RandBool() ? 1.0 : -1.0;
	RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}

