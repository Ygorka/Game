// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPauseWidget.generated.h"

class UButton;

UCLASS()
class SHOOTER_API USPauseWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

	virtual void NativeOnInitialized() override;
private:
	UFUNCTION()
	void OnClearPause();
};
