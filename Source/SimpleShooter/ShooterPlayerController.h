// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	// Returns if the player has successfully received health
	UFUNCTION(BlueprintCallable)
	void HealthPickup(bool& bHasHealed);

	// Returns if the player has successfully received ammo
	UFUNCTION(BlueprintCallable)
	void AmmoPickup(bool& bHasPackedUp);
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;

	UPROPERTY()
	UUserWidget* HUD;

	FTimerHandle RestartTimer;
};
