// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "SMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USGameInstance;
class USLevelItemWidget;

UCLASS()
class SHOOTER_API USMenuWidget : public UUserWidget
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
	
	virtual void NativeOnInitialized() override;

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
