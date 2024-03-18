// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "AIController.h"


UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (ShooterCharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ShooterCharacter->FireWeapon();

    return EBTNodeResult::Succeeded;
}
