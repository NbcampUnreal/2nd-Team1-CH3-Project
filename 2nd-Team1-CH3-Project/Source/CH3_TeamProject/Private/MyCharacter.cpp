#include "MyCharacter.h"
#include "WeaponComponent.h"
#include "WeaponStaticMeshComponent.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"
#include "MyGameStateBase.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp); //, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(CameraComp);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.6f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			// Move
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Move
				);
			}

			// Jump
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartJump
				);
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopJump
				);
			}

			// Look
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Look
				);
			}

			// Sit
			if (PlayerController->SitAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SitAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartSit
				);
				EnhancedInput->BindAction(
					PlayerController->SitAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopSit
				);
			}
			//Sprint
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopSprint
				);
			}
			//Reload
			//if (PlayerController->ReloadAction)
			//{
			//	EnhancedInput->BindAction(
			//		PlayerController->ReloadAction,
			//		ETriggerEvent::Triggered,
			//		this,
			//		&AMyCharacter::ReloadWeapon
			//	);
			//}
		}
	}
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}
void AMyCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void AMyCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);

	AddControllerPitchInput(LookInput.Y);
}

void AMyCharacter::StartSit(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Crouch();
		bIsCronching = true;
	}
}
void AMyCharacter::StopSit(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		UnCrouch();
		bIsCronching = false;
	}
}

void AMyCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void AMyCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventinInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, (TEXT("TakeDamage")));

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventinInstigator, DamageCauser);
	HealthComp->TakeDamage(DamageAmount, DamageEvent, EventinInstigator, DamageCauser);
	if (0 >= HealthComp->GetHealth())
	{
		Dead();
	}
	return Damage;
}

void AMyCharacter::Dead()
{
	if (AMyGameStateBase* MyGameState = GetWorld()->GetGameState<AMyGameStateBase>())
	{
		MyGameState->OnGameOver();
	}
}

//void AMyCharacter::ReloadWeapon()
//{
//	if ()
//	{
//		Reload(); // 재장전 호출
//	}
//}