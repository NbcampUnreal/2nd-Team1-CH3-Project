#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"


AProjectile::AProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnHit called!"));

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) //&& OtherComp->IsSimulatingPhysics())
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation()); //물리적 상호작용 구현 반응 삭제 - 스켈레탈 메시는 물리 시뮬을 기본 지원 x

		float BaseDamage = 10.f;
		UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamage, GetActorForwardVector(), Hit,
			GetInstigatorController(), this, UDamageType::StaticClass());
		//UE_LOG(LogTemp, Warning, TEXT("det"));

		Destroy();
	}
}