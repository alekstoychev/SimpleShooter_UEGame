// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "ShooterPlayerController.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterAIController.h"


void AKillEmAllGameMode::PawnKilled(APawn* Pawn)
{
    Super::PawnKilled(Pawn);
    
    AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(Pawn->GetController());
    if (PlayerController != nullptr)
    {
        EndGame(false);
    }

    for (AShooterAIController* CurrentAI : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!CurrentAI->IsDead())
        {
            return;
        }
    }

    EndGame(true);
}

int AKillEmAllGameMode::CountAllEnemies() const
{
    int count = 0;
    for (AShooterAIController* CurrentAI : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!CurrentAI->IsDead())
        {
            count++;
        }
    }

    return count;
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
