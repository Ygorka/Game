// Shooter. All Rights Reserved.


#include "Player/ShooterBaseCharacter.h"
#include "Components/SCharacterMovementComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

AShooterBaseCharacter::AShooterBaseCharacter(const FObjectInitializer& ObjInit)
:Super(ObjInit.SetDefaultSubobjectClass<USCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UShooterHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>("WeaponComponent");
	

}

void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(GetCharacterMovement());
	check(GetMesh());

	OnHealthChange(HealthComponent->GetHealth(), 0.0f);
	HealthComponent->OnDeath.AddUObject(this, &AShooterBaseCharacter::OnDeath);
	HealthComponent->OnHealthChange.AddUObject(this, &AShooterBaseCharacter::OnHealthChange);

	LandedDelegate.AddDynamic(this, &AShooterBaseCharacter::OnGroundLanded);
}

void AShooterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AShooterBaseCharacter::IsRunning() const
{
	return false;
}

float AShooterBaseCharacter::GetMovementDirection() const
{
	if(GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degress = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degress : Degress  * FMath::Sign(CrossProduct.Z);
}

void AShooterBaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog,Display,TEXT("Player %s is dead"), *GetName());
	
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void AShooterBaseCharacter::OnHealthChange(float Health, float HealthDelta)
{
}

void AShooterBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	UE_LOG(BaseCharacterLog, Display, TEXT("In Landed: %f"),FallVelocityZ);

	if(FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity,LandedDamage,FallVelocityZ);
	UE_LOG(BaseCharacterLog, Display, TEXT("Final Damage: %f"),FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{},nullptr,nullptr);
}

void AShooterBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
	const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	if(!MaterialInst) return;

	MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}



