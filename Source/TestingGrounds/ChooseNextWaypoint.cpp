// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"
#include "ChooseNextWaypoint.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    //Get patrol route
    auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();

    // protect against no patrol route comp
    if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed;  }
    
    // Warn about empty patrol routes
    auto PatrolPoints = PatrolRoute->GetPatrolPoints();

    // protect against empty patrol routes
    if (PatrolPoints.Num() == 0)  
    { 
        UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"));
        return EBTNodeResult::Failed;
    }

    //Set next waypoints
    auto BlackboardComp = OwnerComp.GetBlackboardComponent();
    auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
    BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

    // Cycle index
    auto NextIndex = (Index + 1) % PatrolPoints.Num();
    BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

    UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);
    return EBTNodeResult::Succeeded;  
}
