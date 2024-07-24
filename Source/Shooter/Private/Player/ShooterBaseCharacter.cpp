// Shooter. All Rights Reserved.


#include "Player/ShooterBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SCharacterMovementComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

AShooterBaseCharacter::AShooterBaseCharacter(const FObjectInitializer& ObjInit)
:Super(ObjInit.SetDefaultSubobjectClass<USCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation=true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 180.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent -> SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UShooterHealthComponent>("HealthComponent");
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent -> SetupAttachment(GetRootComponent());
	HealthTextComponent -> SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>("WeaponComponent");
	

}

void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);
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


// Called to bind functionality to input
void AShooterBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(PlayerInputComponent);
	check(WeaponComponent);
	
	PlayerInputComponent->BindAxis("MoveForward",this, &AShooterBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this, &AShooterBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this, &AShooterBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookAround",this ,&AShooterBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed,this,&AShooterBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed,this, &AShooterBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released,this, &AShooterBaseCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UShooterWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::Reload);
}


void AShooterBaseCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	if(Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(),Amount);
}

void AShooterBaseCharacter::MoveRight(float Amount)
{
	if(Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(),Amount);
}

void AShooterBaseCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void AShooterBaseCharacter::OnStopRunning()
{
	WantsToRun = false;
}

bool AShooterBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
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

	//PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);

	if(Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void AShooterBaseCharacter::OnHealthChange(float Health, float HealthDelta)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"),Health)));
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



