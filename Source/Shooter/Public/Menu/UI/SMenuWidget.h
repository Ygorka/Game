// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SBaseWidget.h"
#include "ShooterCoreTypes.h"
#include "SMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USGameInstance;
class USLevelItemWidget;
class USoundCue;

UCLASS()
class SHOOTER_API USMenuWidget : public USBaseWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemsBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* StartGameSound;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HideAnimation;
	
	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	UPROPERTY()
	TArray<USLevelItemWidget*> LevelItemWidgets;
	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();

	void InitLevelItems();
	void OnLevelSelected(const FLevelData& Data);
	USGameInstance* GetSGameInstance() const;
};
