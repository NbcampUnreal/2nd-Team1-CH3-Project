// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UHealthComponent;

UCLASS()
class CH3_TEAMPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Enemy")
	float MaxHealth;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Enemy")
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UHealthComponent* HealthComp;

	void Dead();

public:
	AEnemyCharacter();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
		AController* EventinInstigator, AActor* DamageCauser) override;
};
