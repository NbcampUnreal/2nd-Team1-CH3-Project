// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "MyGameStateBase.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = true;
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);
}

void ASpawnVolume::BeginPlay()
{
    Super::BeginPlay();
}


FVector ASpawnVolume::GetRandomPointInVolume() const
{
    const FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    const FVector BoxOrigin = SpawningBox->GetComponentLocation();

    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

AActor* ASpawnVolume::SpawnEnemy()
{
    if (!EnemyClass) return nullptr;

    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
        EnemyClass,
        GetRandomPointInVolume(),
        FRotator::ZeroRotator
    );
    return SpawnedActor;
}

void ASpawnVolume::SpawnEnemies()
{
    for (int i = 0; i < EnemyCount; i++)
    {
        SpawnEnemy();
		if (AMyGameStateBase* MyGameState = GetWorld()->GetGameState<AMyGameStateBase>())
		{
			MyGameState->SpawnedEnemyCount++;
		}
    }

    if (WaveCount > 0)
    {
        WaveCount--;
        EnemyCount++;
        GetWorldTimerManager().SetTimer(
            WaveTimerHandle,
            this,
            &ASpawnVolume::SpawnEnemies,
            5.0f,
            false
        );
    }
}