// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Enemies\EnemyBase.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Enemies\EEnemyState.h"

void AEnemyBaseController::BeginPlay() {Super::BeginPlay();}

void AEnemyBaseController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    selfEnemyRef = Cast<AEnemyBase>(InPawn);
    if (selfEnemyRef == nullptr) {return;}
    bTree = selfEnemyRef->GetBehaviorTree();
    if (bTree == nullptr) {return;}
    RunBehaviorTree(bTree);
    Blackboard->SetValueAsEnum(TEXT("State"), EEnemyState::IdleE);
    Blackboard->SetValueAsFloat(TEXT("StrafeDistance"), selfEnemyRef->GetStrafeDistance());
    Blackboard->SetValueAsVector(TEXT("InitialLocation"), InPawn->GetActorLocation());
}

void AEnemyBaseController::HandleSensedSight(AActor* sensedActor)
{
    if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Handling Sensed Sight"));}
    Blackboard->SetValueAsObject(TEXT("Target"), sensedActor);
    Blackboard->SetValueAsEnum(TEXT("State"), EEnemyState::ChaseE);
}

void AEnemyBaseController::HandleSensedDamage(AActor* sensedActor)
{
    if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Handling Sensed Damage"));}
    Blackboard->SetValueAsObject(TEXT("Target"), sensedActor);
    Blackboard->SetValueAsEnum(TEXT("State"), EEnemyState::ChaseE);
}

void AEnemyBaseController::Tick(float DeltaTime) {Super::Tick(DeltaTime);}