// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGA_CastPointCollection.h"

FString UAGASGA_CastPointCollection::FormatDamageAbilityDescription(int32 Level, const FString& Description,
	UGameplayAbility* Ability)
{
	FString ParentDescription = Super::FormatDamageAbilityDescription(Level, Description, Ability);
	FStringFormatNamedArguments PointCollectionNamesToValues;
	
	PointCollectionNamesToValues.Add(TEXT("_PointCount0"), FMath::Min(Level, MaxPointCount));
	PointCollectionNamesToValues.Add(TEXT("_PointCount1"), FMath::Min(Level + 1, MaxPointCount));
	
	FString NewProjDescription = FString::Format(*ParentDescription, PointCollectionNamesToValues);
	
	return NewProjDescription;
}
