// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class SHOOTER_API AShooterAICharacter : public AShooterBaseCharacter
{
	GENERATED_BODY()
public:
	AShooterAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	float HealthVisibilityDistance = 1000.0f;

	virtual void BeginPlay() override;
	virtual void OnHealthChange(float Health, float HealthDelta) override;
	virtual void OnDeath() override;
private:
	void UpdateHealthWidgetVisibility();
};
