#include "PickUp_Weapon.h"

UPickUp_Weapon::UPickUp_Weapon()
{
	SphereRadius = 32.f;
}

void UPickUp_Weapon::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPickUp_Weapon::OnSphereBeginOverlap);
}

void UPickUp_Weapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character != nullptr)
	{
		OnPickUp.Broadcast(Character);

		OnComponentBeginOverlap.RemoveAll(this);
	}
}
