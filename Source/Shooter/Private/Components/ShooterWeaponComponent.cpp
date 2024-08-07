// Shooter. All Rights Reserved.


#include "Components/ShooterWeaponComponent.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "GameFramework/Character.h"
#include "ShooterEquipFinishedAnimNotify.h"
#include "Animations/ShooterReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

UShooterWeaponComponent::UShooterWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UShooterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapons"), WeaponNum);

	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

 void UShooterWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for(auto Weapon:Weapons)
	{
		Weapon->DetachFromActor((FDetachmentTransformRules::KeepWorldTransform));
		Weapon->Destroy();
	}

	Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void UShooterWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character || !GetWorld()) return;

	for(auto OneaWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<AShooterBaseWeapon>(OneaWeaponData.WeaponClass);
		if(!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &UShooterWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UShooterWeaponComponent::AttachWeaponToSocket(AShooterBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if(!Weapon || !SceneComponent) return; 
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);
	Weapon->AttachToComponent(SceneComponent,AttachmentRules,SocketName);
}

void UShooterWeaponComponent::EquipWeapon (int32 WeaponIndex)
{
	if(WeaponIndex< 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Display, TEXT("Invalid weapon index"));
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character) return;

	if(CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data)
	{
		return Data.WeaponClass == CurrentWeapon->GetClass();
	});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void UShooterWeaponComponent::StartFire()
{
	if(!CanFire()) return;
	CurrentWeapon->StartFire();
}

void UShooterWeaponComponent::StopFire()
{
	if(!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void UShooterWeaponComponent::NextWeapon()
{
	if(!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void UShooterWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character) return;

	Character->PlayAnimMontage(Animation);
}

void UShooterWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UShooterEquipFinishedAnimNotify>(EquipAnimMontage);
	if(EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim is forgotten to set"));
		checkNoEntry();
	}

	for(auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShooterReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if(!ReloadFinishedNotify){
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim is forgotten to set"));
			checkNoEntry();
		}
		
		ReloadFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnReloadFinished);
	}
}
void UShooterWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character || Character->GetMesh() != MeshComponent) return;
	EquipAnimInProgress = false;
}

void UShooterWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character || Character->GetMesh() != MeshComponent) return;
	ReloadAnimInProgress = false;
}

bool UShooterWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool UShooterWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UShooterWeaponComponent::CanReload() const
{
	return CurrentWeapon
	&& !EquipAnimInProgress
	&& !ReloadAnimInProgress
	&& CurrentWeapon->CanReload();
}

void UShooterWeaponComponent::Reload()
{
	ChangeClip();
}

void UShooterWeaponComponent::OnEmptyClip(AShooterBaseWeapon* AmmoEmptyWeapon)
{
	if(!AmmoEmptyWeapon) return;
	
	if(CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for(const auto Weapon : Weapons)
		{
			if(Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}
void UShooterWeaponComponent::ChangeClip()
{
	if(!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

bool UShooterWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if(CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool UShooterWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if(CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool UShooterWeaponComponent::TryToAddAmmo(TSubclassOf<AShooterBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for(const auto Weapon : Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}

bool UShooterWeaponComponent::NeedAmmo(TSubclassOf<AShooterBaseWeapon> WeaponType)
{
	for(const auto Weapon : Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return false;
}
