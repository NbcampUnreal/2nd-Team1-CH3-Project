// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UHealthComponent;
class USphereComponent;
class APatrolPath;
UCLASS()
class CH3_TEAMPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UHealthComponent* HealthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USphereComponent* HeadCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USphereComponent* HandCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	APatrolPath* PatrolPath;


	FTimerHandle DestroyTimerHandle;

	void Dead();

	void RemoveBody();

public:
	AEnemyCharacter();

	UFUNCTION(BlueprintCallable, Category = "HandCollider")
	void EnableHandCollider();
	UFUNCTION(BlueprintCallable, Category = "HandCollider")
	void DisableHandCollider();

	bool IsPlayerHit;

	APatrolPath* GetPatrolPath();

	UFUNCTION()
	void OnHandOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
		AController* EventinInstigator, AActor* DamageCauser) override;
};
