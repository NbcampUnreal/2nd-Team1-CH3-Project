// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	TotalScore = 0;
	ZombieKillCount = 0;
	SoldierKillCount = 0;
	TotalKillCount = 0;
	CurrentLevelIndex = 0;
}

void UMyGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
}

void UMyGameInstance::AddToKillCount(FString EnemyType)
{
	if (EnemyType == "Zombie")
	{
		ZombieKillCount++;
	}
	else if (EnemyType == "Soldier")
	{
		SoldierKillCount++;
	}
	TotalKillCount++;
}
