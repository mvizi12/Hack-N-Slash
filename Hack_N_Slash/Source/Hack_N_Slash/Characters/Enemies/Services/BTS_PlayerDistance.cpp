// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PlayerDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UBTS_PlayerDistance::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    FVector currentLoc {OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()};
    FVector playerLoc {UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()};
    float distance {static_cast<float>(FVector::Distance(currentLoc, playerLoc))};
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("DistanceToTarget"), distance);
}