// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AWeapon;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	bool IsCharacterDead() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	// Returns if the player has successfully received health
	bool HealthPickup();

	// Returns if the player has successfully received health
	bool AmmoPickup();

	void FireWeapon();
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void GetAmmoInfo(int& OutCurrentAmmo, int& OutTotalAmmo) const;
	
private:

	void MoveForward(float Input);
	void MoveSideways(float Input);
	void LookUpRate(float Input);
	void LookSidewaysRate(float Input);

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere)
	float CurrentHealth = MaxHealth;

	UPROPERTY(EditAnywhere)
	float RotateRate = 10;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY()
	AWeapon* Weapon;

};