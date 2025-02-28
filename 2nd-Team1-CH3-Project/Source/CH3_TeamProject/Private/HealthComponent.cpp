// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Engine/GameEngine.h"
#include "Engine/DamageEvents.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{

}

void UHealthComponent::InitializeHealth(float health, float maxHealth)
{
	maxHealth = MaxHealth;
	Health = (MaxHealth > health) ? health : MaxHealth;
}

void UHealthComponent::SetHealth(float health, float maxHealth)
{
	MaxHealth = (maxHealth == -1) ? MaxHealth : maxHealth;
	Health = (MaxHealth > health) ? health : MaxHealth;
}

float UHealthComponent::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventinInstigator, AActor* DamageCauser)
{
	float Damage = DamageAmount;
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (0 == (PointDamageEvent->HitInfo.BoneName).Compare(FName(TEXT("head"))))
		{
			Damage *= 5;
		}
	}
	Health -= Damage;
	return Damage;
}

float UHealthComponent::GetHealth()
{
	return Health;
}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}