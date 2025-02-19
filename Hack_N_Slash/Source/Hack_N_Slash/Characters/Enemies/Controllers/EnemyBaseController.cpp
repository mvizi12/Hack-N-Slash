// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Enemies\EnemyBase.h"

void AEnemyBaseController::BeginPlay() {Super::BeginPlay();}

void AEnemyBaseController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    selfEnemyRef = Cast<AEnemyBase>(InPawn);
    if (selfEnemyRef == nullptr) {return;}
    RunBehaviorTree(selfEnemyRef->GetBehaviorTree());
    blackBoardComp = GetBlackboardComponent();
    blackBoardComp->SetValueAsObject(TEXT("Target"), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AEnemyBaseController::Tick(float DeltaTime) {Super::Tick(DeltaTime);}