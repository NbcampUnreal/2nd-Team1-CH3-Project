// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Engine/GameEngine.h"
#include "Engine/DamageEvents.h"
#include "HealthComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("SpringArm"));
	
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventinInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventinInstigator, DamageCauser);
	HealthComp->TakeDamage(DamageAmount, DamageEvent, EventinInstigator, DamageCauser);
	if (0 >= HealthComp->GetHealth())
	{
		Dead();
	}
	return Damage;
}

void AEnemyCharacter::Dead()
{
	Destroy();
}