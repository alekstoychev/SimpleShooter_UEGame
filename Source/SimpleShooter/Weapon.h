// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SIMPLESHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmoAmount() const;
	UFUNCTION(BlueprintCallable)
	int GetTotalAmmoAmount() const;

	bool AmmoPickup();

	void Reload();

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFlash;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* MuzzleSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ImpactSound;
	
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	float MaxRange = 10000;

	UPROPERTY(EditAnywhere)
	float Damage = 25;

	bool AimTrace(FHitResult& OutHit, FVector& OutShotDirection);

	AController* GetOwnerController() const;

	
	UPROPERTY(EditDefaultsOnly)
	int MaxAmmo = 120;
	UPROPERTY(VisibleAnywhere)
	int TotalAmmo;

	UPROPERTY(EditDefaultsOnly)
	int MagSize = 30;
	UPROPERTY(VisibleAnywhere)
	int CurrentAmmo;

	UPROPERTY(EditDefaultsOnly)
	float ReloadSpeed;

	FTimerHandle ReloadTimer;
};
