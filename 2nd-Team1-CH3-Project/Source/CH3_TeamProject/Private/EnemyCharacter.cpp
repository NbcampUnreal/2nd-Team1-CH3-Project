// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Engine/GameEngine.h"
#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "MyCharacter.h"
#include "PatrolPath.h"
#include "MyGameStateBase.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	HeadCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollider"));
	HeadCollider->SetupAttachment(GetMesh(), FName("head"));

	HandCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HandCollider"));
	HandCollider->SetupAttachment(GetMesh(), FName("righthand"));
	HandCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HandCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHandOverlap);
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
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (AMyGameStateBase* MyGameState = GetWorld()->GetGameState<AMyGameStateBase>())
	{
		MyGameState->OnEnemyDead();
	}

	// AI 컨트롤러 비활성화
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->UnPossess();
	}

	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AEnemyCharacter::RemoveBody,
		10.0f,
		false
	);
}

void AEnemyCharacter::RemoveBody()
{
	Destroy();
}

void AEnemyCharacter::EnableHandCollider()
{
	HandCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	IsPlayerHit = false;
}

void AEnemyCharacter::DisableHandCollider()
{
	HandCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyCharacter::OnHandOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	
	if (OtherActor && OtherActor != this)
	{
		ACharacter* Player = Cast<ACharacter>(OtherActor);
		if (Player && Player->IsA(AMyCharacter::StaticClass()) && !IsPlayerHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit"));
			IsPlayerHit = true;
			UGameplayStatics::ApplyDamage(Player, 20.0f, GetController(), this, UDamageType::StaticClass());
		}
	}
}

APatrolPath* AEnemyCharacter::GetPatrolPath()
{
	return PatrolPath;
}