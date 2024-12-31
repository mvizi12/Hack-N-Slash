// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

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