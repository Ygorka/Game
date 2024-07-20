#pragma once

#include "ShooterCoreTypes.generated.h"
//Weapon
class AShooterBaseWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, AShooterBaseWeapon*);

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon", meta = (EditCondition="!Infinite"))
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AShooterBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Weapon")
	UAnimMontage* ReloadAnimMontage;
	
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="UI")
	UTexture2D* MainIcon;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="UI")
	UTexture2D* CrossHairIcon;
	
};
//Health
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangeSignature, float);

