// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AGASInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAS_API UAGASInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAGASInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
		ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAGASInputComponent::BindAbilityActions(const UAGASInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const auto& [InputAction, InputTag] : InputConfig->AbilityInputActions)
	{
		if (InputAction == nullptr || !InputTag.IsValid()) return;

		if (PressedFunc)
		{
			BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
		}

		if (ReleasedFunc)
		{
			BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag);
		}

		if (HeldFunc)
		{
			BindAction(InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag);
		}
	}
}
