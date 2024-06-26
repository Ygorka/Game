// Shooter. All Rights Reserved.


#include "Player/ShooterBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SCharacterMovementComponent.h"


// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter(const FObjectInitializer& ObjInit)
:Super(ObjInit.SetDefaultSubobjectClass<USCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation=true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

}

// Called when the game starts or when spawned
void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&AShooterBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AShooterBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this,&AShooterBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookAround",this,&AShooterBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed,this,&AShooterBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed,this, &AShooterBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released ,this, &AShooterBaseCharacter::OnStopRunning);
	
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



