// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "AGASDamageTextComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAS_API UAGASDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);
};
