// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class CH3_TEAMPROJECT_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
public:
    APatrolPath();

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Patrol")
    TArray<AActor*> Waypoints;

    AActor* GetWaypoint(int32 Index) const;

    int32 Num() const;
};
