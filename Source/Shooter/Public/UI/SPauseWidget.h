// Shooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SBaseWidget.h"
#include "SPauseWidget.generated.h"

class UButton;

UCLASS()
class SHOOTER_API USPauseWidget : public USBaseWidget
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
