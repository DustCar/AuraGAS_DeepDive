// Copyright Dustin Carmelo


#include "AbilitySystem/Data/AGASLootTiers.h"

TArray<FLootItem> UAGASLootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnItems;
	
	for (FLootItem& Item : LootItems)
	{
		for (int32 i = 0; i < Item.MaxDropNumber; i++)
		{
			if (FMath::FRandRange(1.f, 100.f) < Item.DropChance)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				ReturnItems.Add(NewItem);
			}
		}
	}
	
	return ReturnItems;
}
