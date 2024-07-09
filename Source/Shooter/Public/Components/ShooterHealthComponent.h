// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShooterHealthComponent();

	FOnDeathSignature OnDeath;
	FOnHealthChangeSignature OnHealthChange;
	
	float GetHealth() const {return Health;}
	
	UFUNCTION(BlueprintCallable)
	bool IsDead() const {return FMath::IsNearlyZero(Health);}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMax="100.0", ClampMin="0.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal")
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(EditCondition="AutoHeal"))
	float HealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(EditCondition="AutoHeal"))
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(EditCondition="AutoHeal"))
	float HealModifier = 5.0f;
	
	
	virtual void BeginPlay() override;

private:
	float Health = 0.0f;
	FTimerHandle HealTimerHandle;
	
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);
		
};
