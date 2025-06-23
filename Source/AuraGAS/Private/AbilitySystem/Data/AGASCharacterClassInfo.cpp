// Copyright Dustin Carmelo


#include "AbilitySystem/Data/AGASCharacterClassInfo.h"

FCharacterClassDefaultInfo UAGASCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
