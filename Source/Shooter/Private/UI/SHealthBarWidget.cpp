// Shooter. All Rights Reserved.


#include "UI/SHealthBarWidget.h"
#include "Components/ProgressBar.h"

void USHealthBarWidget::SetHealthPercent(float Percent)
{
    if(!HealthProgressBar) return;

    const auto HealthBarVisible = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent))//
    ? ESlateVisibility::Hidden
    : ESlateVisibility::Visible;

    HealthProgressBar->SetVisibility(HealthBarVisible);

    const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
    HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);

    HealthProgressBar->SetPercent(Percent);
}