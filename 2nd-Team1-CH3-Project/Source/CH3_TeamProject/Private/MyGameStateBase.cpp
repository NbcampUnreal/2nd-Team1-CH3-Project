// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "MyGameInstance.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EnemyCharacter.h"

AMyGameStateBase::AMyGameStateBase()
{
	Score = 0;
	SpawnedEnemyCount = 0;
	DeadEnemyCount = 0;
	WaveDuration = 0.0f;
	CurrentWaveIndex = 0;
	MaxWaveIndex = 0;
	CurrentLevelIndex = 0;
	MaxLevelIndex = 1;
	GateIndex = 0;
	bIsGameOver = false;
}

void AMyGameStateBase::BeginPlay()
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundActors);

	SpawnedEnemyCount = FoundActors.Num();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMyGameStateBase::UpdateHUD,
		0.1f,
		true
	);
}

int32 AMyGameStateBase::GetScore() const
{
	return Score;
}

int32 AMyGameStateBase::GetKillCount() const
{
	return DeadEnemyCount;
}

bool AMyGameStateBase::GetbIsGameOver() const
{
	return bIsGameOver;
}

void AMyGameStateBase::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			MyGameInstance->AddToScore(Amount);
		}
	}
}

void AMyGameStateBase::AddKillCount()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			MyGameInstance->AddToKillCount();
		}
	}
}

void AMyGameStateBase::OnGameOver()
{
	UpdateMainMenu();

	bIsGameOver = true;

	//게임오버 화면 출력, 조작 중지
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true);
			MyPlayerController->ShowMainMenu(true);
		}
	}
}

void AMyGameStateBase::StartLevel()
{
	//게임 HUD 표시
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->ShowGameHUD();
		}
	}

	//현재 레벨 인덱스 반영
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
		}
	}
}

void AMyGameStateBase::EndLevel()
{
	UpdateMainMenu();

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			AddScore(Score);

			CurrentLevelIndex++;
			MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex > MaxLevelIndex)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
}

void AMyGameStateBase::StartWave()
{
	/*TArray<AActor*> FoundZombieSpawnVolumes;
	const int32 ZombieToSpawn = 0 * CurrentWaveIndex * 0;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("ZombieSpawn"), FoundZombieSpawnVolumes);;
	if (GateIndex >= 0)
	{
		for (int32 i = 0; i < ZombieToSpawn; i++)
		{
			if (FoundZombieSpawnVolumes.Num() > 0)
			{
				ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundZombieSpawnVolumes[0]);
				if (SpawnVolume)
				{
					AActor* SpawnedEnemy = SpawnVolume->SpawnZombies();
					SpawnedEnemyCount++;
				}
			}
		}
	}

	TArray<AActor*> FoundSoldierSpawnVolumes;
	const int32 SoldierToSpawn = 0 * CurrentWaveIndex * 0;

	if (GateIndex >= 0)
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SoilderSpawn"), FoundSoldierSpawnVolumes);;

		for (int32 i = 0; i < SoldierToSpawn; i++)
		{
			if (FoundSoldierSpawnVolumes.Num() > 0)
			{
				ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundSoldierSpawnVolumes[0]);
				if (SpawnVolume)
				{
					AActor* SpawnedEnemy = SpawnVolume->SpawnSoldiers();
					SpawnedEnemyCount++;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&AMyGameStateBase::OnWaveTimeUp,
		WaveDuration * 60.0f,
		false
	);*/
}

void AMyGameStateBase::EndWave()
{
	/*GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	CurrentWaveIndex++;

	if (CurrentWaveIndex >= MaxWaveIndex)
	{
		return;
	}

	if (CurrentWaveIndex < MaxWaveIndex)
	{
		StartWave();
	}*/
}

void AMyGameStateBase::OnWaveTimeUp()
{
	/*EndWave();*/
}

void AMyGameStateBase::OnEnemyDead()
{
	DeadEnemyCount++;
	AddKillCount();
}

void AMyGameStateBase::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
						if (MyGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* KillCountText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("EnemyCountText"))))
				{
					KillCountText->SetText(FText::FromString(FString::Printf(TEXT("Enemy Killed: %d"), DeadEnemyCount)));
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("LevelText"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}

void AMyGameStateBase::UpdateMainMenu()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* MainMenuWidget = MyPlayerController->GetMainMenuWidget())
			{

				if (UTextBlock* TotalKillCountText = Cast<UTextBlock>(MainMenuWidget->GetWidgetFromName(TEXT("TotalKillCountText"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
						if (MyGameInstance)
						{
							TotalKillCountText->SetText(FText::FromString(FString::Printf(TEXT("Total Kill: %d"), MyGameInstance->TotalKillCount)));
						}
					}
				}
				
			}
		}
	}
}
