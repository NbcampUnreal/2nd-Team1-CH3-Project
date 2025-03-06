// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3_TEAMPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Health;

public:	
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventinInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth();
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth();
	void InitializeHealth(float health, float maxHealth);
	void SetHealth(float health, float maxHealth = -1);
};
