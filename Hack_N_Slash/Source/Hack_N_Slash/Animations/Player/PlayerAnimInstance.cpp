// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

/*PRIVATE FUNCTIONS*/
void UPlayerAnimInstance::UpdateVelocity()
{
    if (!IsValid(playerPawn)) {return;}

    currentVeloctiy = playerPawn->GetVelocity();
}


/*PROTECTED FUNCTIONS*/
void UPlayerAnimInstance::UpdatePlayerPawn()
{
    playerPawn = TryGetPawnOwner();
    characterRef = Cast<ACharacter>(playerPawn);
    movementComp = characterRef->GetCharacterMovement();
}

void UPlayerAnimInstance::UpdateCurrentAngle()
{
    if (!IsValid(playerPawn)) {return;}

    UpdateVelocity();
    FRotator currentRotation = playerPawn->GetActorRotation();
    currentAngle = CalculateDirection(currentVeloctiy, currentRotation);
    //UKismetAnimationLibrary::CalculateDirection(currentVeloctiy, currentRotation);
}

void UPlayerAnimInstance::UpdateCurrentSpeed()
{
    if (!IsValid(playerPawn)) {return;}

    UpdateVelocity();
    currentSpeed = static_cast<float>(currentVeloctiy.Length());
}

void UPlayerAnimInstance::UpdateIsFalling()
{
    if (!IsValid(playerPawn)) {return;}

    bIsFalling = movementComp->IsFalling();
}
