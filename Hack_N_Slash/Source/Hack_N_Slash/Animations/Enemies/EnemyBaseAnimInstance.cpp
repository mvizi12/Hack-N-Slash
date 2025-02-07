// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

/*PRIVATE FUNCTIONS*/
void UEnemyBaseAnimInstance::UpdateVelocity()
{
    currentVeloctiy = enemyPawn->GetVelocity();
}

/*PROTECTED FUNCTIONS*/
void UEnemyBaseAnimInstance::UpdateEnemyPawn()
{
    enemyPawn = TryGetPawnOwner();
    characterRef = Cast<ACharacter>(enemyPawn);
    movementComp = characterRef->GetCharacterMovement();
}

void UEnemyBaseAnimInstance::UpdateCurrentSpeed()
{
    if (!IsValid(enemyPawn)) {return;}

    UpdateVelocity();
    currentSpeed = static_cast<float>(currentVeloctiy.Length());
}

void UEnemyBaseAnimInstance::UpdateIsFalling()
{
    if (!IsValid(enemyPawn)) {return;}
    bIsFalling = movementComp->IsFalling();
}