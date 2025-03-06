// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;
UCLASS()
class CH3_TEAMPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<AActor> EnemyClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float EnemyCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float WaveCount;

    FTimerHandle WaveTimerHandle;

    FVector GetRandomPointInVolume() const;
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    AActor* SpawnEnemy();
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void SpawnEnemies();
protected:
    virtual void BeginPlay() override;
};
