// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Chase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h" //Need for the "MoveTo" function for some reason
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Enemies\Controllers\EnemyBaseController.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Enemies\EEnemyState.h"

UBTT_Chase::UBTT_Chase()
{
    bNotifyTick = true;
    FinishChaseDelegate.BindUFunction(this, "FinishChaseTask");
}

EBTNodeResult::Type UBTT_Chase::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    bIsFinished = false;
    controllerRef = Cast<AEnemyBaseController>(OwnerComp.GetAIOwner());
    if (controllerRef == nullptr) {return EBTNodeResult::Failed;}

    float distanceToTarget {OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToTarget"))};
    float strafeDistance {OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("StrafeDistance"))};
    if (distanceToTarget < strafeDistance) //If in stafe range, abort to starfe state
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("State"), EEnemyState::StrafeE);
        Abort(OwnerComp, NodeMemory);
    }
    //Else chase the target
    AActor* targetRef {Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")))};
    FAIMoveRequest moveRequest {targetRef};
    moveRequest.SetUsePathfinding(true);
    moveRequest.SetAcceptanceRadius(acceptanceRadius);

    controllerRef->MoveTo(moveRequest);
    controllerRef->SetFocus(targetRef);
    controllerRef->ReceiveMoveCompleted.AddUnique(FinishChaseDelegate);
    return EBTNodeResult::InProgress;
}

void UBTT_Chase::TickTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    //Add abort logic if lost sense of target?
    if (!bIsFinished) {return;}
    controllerRef->ReceiveMoveCompleted.Remove(FinishChaseDelegate);
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_Chase::Abort(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    controllerRef->StopMovement();
    controllerRef->ClearFocus(EAIFocusPriority::Gameplay);
    controllerRef->ReceiveMoveCompleted.Remove(FinishChaseDelegate);
    AbortTask(OwnerComp, NodeMemory);
    FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
}

void UBTT_Chase::FinishChaseTask() {bIsFinished = true;}