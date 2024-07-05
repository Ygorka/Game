// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterBaseWeapon.generated.h"

class USkeletalMeshComponent;
UCLASS()
class SHOOTER_API AShooterBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterBaseWeapon();
	virtual void Fire();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TraceMaxDistance = 1500.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Damage")
	float DamageAmount = 10.0f;
	
	void MakeDamage(const FHitResult& HitResult, APlayerController* Controller);
	void MakeShoot();
};
