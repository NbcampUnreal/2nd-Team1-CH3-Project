#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "MyCharacter.h"
#include "PickUp_Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AMyCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CH3_TEAMPROJECT_API UPickUp_Weapon : public USphereComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUp OnPickUp;

	UPickUp_Weapon();
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
