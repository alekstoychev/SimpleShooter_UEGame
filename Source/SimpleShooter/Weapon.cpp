// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	CurrentAmmo = MagSize;
	TotalAmmo = MaxAmmo;
	ReloadSpeed = 1.f;
}

void AWeapon::Fire()
{
	if (CurrentAmmo == 0)
	{
		return;
	}

	CurrentAmmo--;
	if (CurrentAmmo < 0)
	{
		CurrentAmmo = 0;
	}

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;
	bool bSuccess = AimTrace(HitResult, ShotDirection);
	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
		
		
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWeapon::AimTrace(FHitResult& OutHit, FVector& OutShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	OwnerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	OutShotDirection = -ViewRotation.Vector();

	FVector End = ViewLocation + ViewRotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OutHit, ViewLocation, End, ECC_GameTraceChannel1, Params);
}

AController* AWeapon::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	
	return OwnerPawn->GetController();
}

// Ammunition related
int AWeapon::GetCurrentAmmoAmount() const
{
	return CurrentAmmo;
}

int AWeapon::GetTotalAmmoAmount() const
{
	return TotalAmmo;
}

bool AWeapon::AmmoPickup()
{
	if (TotalAmmo < MaxAmmo)
	{
		TotalAmmo = MaxAmmo;
		return true;
	}

	return false;
}

void AWeapon::Reload()
{
	int AmmoToLoad = MagSize - CurrentAmmo;

	if (AmmoToLoad > 0)
	{
		if (TotalAmmo >= AmmoToLoad)
		{
			CurrentAmmo = MagSize;
			TotalAmmo -= AmmoToLoad;
		}
		else
		{
			CurrentAmmo += TotalAmmo;
			TotalAmmo = 0;
		}
	}
}