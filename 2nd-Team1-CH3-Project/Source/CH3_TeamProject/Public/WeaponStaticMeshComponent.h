#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
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
	UNiagaraSystem* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	UWeaponStaticMeshComponent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeapon(AMyCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
	void StartFiring();
	void StopFiring();
	void Reload();
	void CompleteReload();

protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;
	int32 MaxAmmo;

private:
	AMyCharacter* Character;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;

	float ReloadTime;
	bool bIsReloading;

	float FireRate;
	bool bIsFiring;
};