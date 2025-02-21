#include "BTT_PerformAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Enemies\Controllers\EnemyBaseController.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Enemies\EEnemyState.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Enemy.h"

UBTT_PerformAttack::UBTT_PerformAttack() {bNotifyTick = true;}

EBTNodeResult::Type UBTT_PerformAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    bIsFinished = false;
    controllerRef = Cast<AEnemyBaseController>(OwnerComp.GetAIOwner());
    if (controllerRef == nullptr) {return EBTNodeResult::Failed;}

    IEnemy* iEnemyRef {Cast<IEnemy>(controllerRef->GetCharacter())};
    if (iEnemyRef == nullptr) {Abort(OwnerComp, NodeMemory);}

    float distanceToTarget {OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToTarget"))};
    float strafeDistance {OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("StrafeDistance"))};
    if (distanceToTarget > strafeDistance)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("State"), EEnemyState::ChaseE);
        Abort(OwnerComp, NodeMemory);
    }

    //AActor* targetRef {Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")))};
    //controllerRef->SetFocus(targetRef);
    iEnemyRef->Attack(bRanged);

    FTimerHandle AttackTimerHandle;
    controllerRef->GetCharacter()->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UBTT_PerformAttack::FinishAttackTask, iEnemyRef->GetAnimationDuration(), false);

    return EBTNodeResult::InProgress;
}

void UBTT_PerformAttack::TickTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    if (!bIsFinished) {return;}
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_PerformAttack::Abort(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    //controllerRef->StopMovement();
    //controllerRef->ClearFocus(EAIFocusPriority::Gameplay);
    AbortTask(OwnerComp, NodeMemory);
    FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
}

void UBTT_PerformAttack::FinishAttackTask() {bIsFinished = true;}