// Copyright Dustin Carmelo


#include "Actor/AGASEnemySpawnPoint.h"

#include "Characters/AGASEnemy.h"


void AAGASEnemySpawnPoint::SpawnEnemy()
{
	
	AAGASEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAGASEnemy>(EnemyClass, GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	IAGASCombatInterface::Execute_SetCharacterLevel(Enemy, EnemyLevel);
	
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}

void AAGASEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

