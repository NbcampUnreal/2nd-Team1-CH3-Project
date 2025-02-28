#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponStaticMeshComponent.generated.h"

class AMyCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CH3_TEAMPROJECT_API UWeaponStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UWeaponStaticMeshComponent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeapon(AMyCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	//UFUNCTION()
	//void ResetFire();

protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	AMyCharacter* Character;

	//UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	//float FireRate;

	//FTimerHandle FireTimerHandle;
};