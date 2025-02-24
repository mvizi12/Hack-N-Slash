#include "BTT_Strafe.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h" //Need for the "MoveTo" function for some reason
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Enemies\Controllers\EnemyBaseController.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Enemies\EEnemyState.h"

UBTT_Strafe::UBTT_Strafe()
{
    bNotifyTick = true;
    FinishStrafeDelegate.BindUFunction(this, "FinishStrafeTask");
}

EBTNodeResult::Type UBTT_Strafe::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    bIsFinished = false;
    controllerRef = Cast<AEnemyBaseController>(OwnerComp.GetAIOwner());
    if (controllerRef == nullptr) {return EBTNodeResult::Failed;}

    float distanceToTarget {OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToTarget"))};
    float strafeDistance {OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("StrafeDistance"))};
    if (distanceToTarget > strafeDistance)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("State"), EEnemyState::ChaseE);
        Abort(OwnerComp, NodeMemory);
    }

    AActor* targetRef {Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")))};
    FVector targetLoc {targetRef->GetActorLocation()};
    //UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    FNavLocation randomPoint;
    if (navSystem && navSystem->GetRandomPointInNavigableRadius(targetLoc, strafeDistance, randomPoint))
    {
        //randomPoint.Location now contains the random location
        //Use randomPoint.Location for your logic (e.g., move an AI agent)
        FAIMoveRequest moveRequest {randomPoint.Location};
        moveRequest.SetUsePathfinding(true);
        moveRequest.SetAcceptanceRadius(acceptanceRadius);
    
        controllerRef->MoveTo(moveRequest);
        controllerRef->SetFocus(targetRef);
        controllerRef->ReceiveMoveCompleted.AddUnique(FinishStrafeDelegate);
        return EBTNodeResult::InProgress;
    }
    else
    {
        //Handle the case where a random point couldn't be found
        //(e.g., no navigable area within the radius)
        if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Failed Strafe Task"));}
        return EBTNodeResult::Failed;
    }
}

void UBTT_Strafe::TickTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    //Add abort logic if lost sense of target?
    if (!bIsFinished) {return;}
    //if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Finished"));}
    controllerRef->ReceiveMoveCompleted.Remove(FinishStrafeDelegate);

    double randomValue {UKismetMathLibrary::RandomFloat()};
    if (randomValue > threshold)
    {
        threshold = 0.9;
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("State"), EEnemyState::AttackE);
        Abort(OwnerComp, NodeMemory);
    }
    else {threshold -= 0.1;}

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_Strafe::Abort(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    controllerRef->StopMovement();
    controllerRef->ClearFocus(EAIFocusPriority::Gameplay);
    controllerRef->ReceiveMoveCompleted.Remove(FinishStrafeDelegate);
    AbortTask(OwnerComp, NodeMemory);
    FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
}

void UBTT_Strafe::FinishStrafeTask() {bIsFinished = true;}