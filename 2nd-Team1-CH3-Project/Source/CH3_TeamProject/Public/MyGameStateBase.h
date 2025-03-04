// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAMPROJECT_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMyGameStateBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 SpawnedEnemyCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 DeadEnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 CurrentWaveIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWaveIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 GateIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevelIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FName> LevelMapNames;
	
	bool bIsGameOver;

	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetKillCount() const;
	UFUNCTION(BlueprintPure, Category = "GameState")
	bool GetbIsGameOver() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddKillCount(FString EnemyType);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void EndLevel();
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartWave();
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void EndWave();

	void OnWaveTimeUp();
	void OnEnemyDead(TArray<FName> EnemyType);
	void UpdateHUD();
	void UpdateMainMenu();
};
