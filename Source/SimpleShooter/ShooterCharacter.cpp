// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Weapon.h"
#include "SimpleShooterGameModeBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Weapon->SetOwner(this);
	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSideways"), this, &AShooterCharacter::MoveSideways);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookSideways"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookSidewaysRate"), this, &AShooterCharacter::LookSidewaysRate);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::FireWeapon);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AShooterCharacter::ReloadWeapon);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{   
	 
	if (CurrentHealth == 0.f)
	{
		return 0.f;
	}

	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth -= DamageApplied;
	if (CurrentHealth < 0.f)
	{
		CurrentHealth = 0.f;
	}

	if (IsCharacterDead())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
	}

	return DamageApplied;
}

bool AShooterCharacter::IsCharacterDead() const
{
	return CurrentHealth == 0.f;
}

float AShooterCharacter::GetHealthPercent() const
{
	return CurrentHealth / MaxHealth;
}

bool AShooterCharacter::HealthPickup()
{
	if (CurrentHealth < MaxHealth)
	{
		CurrentHealth += 50;
		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
		return true;
	}

	return false;
}

bool AShooterCharacter::AmmoPickup()
{
	return Weapon->AmmoPickup();
}

void AShooterCharacter::MoveForward(float Input)
{
	AddMovementInput(GetActorForwardVector() * Input);
}

void AShooterCharacter::MoveSideways(float Input)
{
	AddMovementInput(GetActorRightVector() * Input);
}

void AShooterCharacter::LookUpRate(float Input)
{
	AddControllerPitchInput(Input * RotateRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookSidewaysRate(float Input)
{
	AddControllerYawInput(Input * RotateRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::FireWeapon()
{
	Weapon->Fire();
}

void AShooterCharacter::ReloadWeapon()
{
	Weapon->Reload();
}

void AShooterCharacter::GetAmmoInfo(int& OutCurrentAmmo, int& OutTotalAmmo) const
{
	OutCurrentAmmo = Weapon->GetCurrentAmmoAmount();
	OutTotalAmmo = Weapon->GetTotalAmmoAmount();
}