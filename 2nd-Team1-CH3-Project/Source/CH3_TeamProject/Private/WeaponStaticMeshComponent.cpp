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
#include "InputAction.h"
#include "Engine/Engine.h"

UWeaponStaticMeshComponent::UWeaponStaticMeshComponent()
{
	MuzzleOffset = FVector(0.0f, 0.0f, 20.0f);
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	FireRate = 0.1f;
	bIsFiring = false;
}

void UWeaponStaticMeshComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (CurrentAmmo <= 0) // 탄창 확인
	{
		//UE_LOG(LogTemp, Warning, TEXT("Out of ammo!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("out of ammo!"));
		return;
	}

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

	CurrentAmmo--;

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

}

void UWeaponStaticMeshComponent::Reload()
{
	if (CurrentAmmo < MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
		//UE_LOG(LogTemp, Warning, TEXT("Reloading..."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Reloading..."));
	}
}

bool UWeaponStaticMeshComponent::AttachWeapon(AMyCharacter* TargetCharacter)
{
	if (!TargetCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetCharacter is null!"));
		return false;
	}

	Character = TargetCharacter;

	// PlayerController
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null!"));
		return false;
	}

	// EnhancedInputComponent
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponStaticMeshComponent::StartFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UWeaponStaticMeshComponent::StopFiring);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UWeaponStaticMeshComponent::Reload);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent is null!"));
		return false;
	}

	// 무기 메시
	if (Character->GetInstanceComponents().FindItemByClass<UWeaponStaticMeshComponent>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon already attached!"));
		return false;
	}

	// 무기 장착
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// IMC
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(FireMappingContext, 1);
	}

	return true;
}

void UWeaponStaticMeshComponent::StartFiring()
{
	if (bIsFiring) return; // 발사 중 중복 발사 금지
	bIsFiring = true;
	Fire();
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UWeaponStaticMeshComponent::Fire, FireRate, true); // 발사 타이머 설정
}

// 발사 중지 함수
void UWeaponStaticMeshComponent::StopFiring()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
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