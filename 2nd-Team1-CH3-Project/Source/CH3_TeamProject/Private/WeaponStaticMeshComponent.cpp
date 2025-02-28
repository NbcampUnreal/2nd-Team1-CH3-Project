#include "WeaponStaticMeshComponent.h"
#include "MyCharacter.h"
#include "Projectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "TimerManager.h"

UWeaponStaticMeshComponent::UWeaponStaticMeshComponent()
{
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	//FireRate = 0.5f;
}

void UWeaponStaticMeshComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	//if (GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	//{
	//	return;
	//}

	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	if (FireAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	//GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UWeaponStaticMeshComponent::ResetFire, FireRate, false);

}

//void UWeaponStaticMeshComponent::ResetFire()
//{
//	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
//}


bool UWeaponStaticMeshComponent::AttachWeapon(AMyCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UWeaponStaticMeshComponent>())
	{
		return false;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponStaticMeshComponent::Fire);
		}
	}

	return true;
}

void UWeaponStaticMeshComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character != nullptr)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}