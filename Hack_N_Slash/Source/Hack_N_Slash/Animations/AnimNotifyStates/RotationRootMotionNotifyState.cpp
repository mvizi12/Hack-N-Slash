// Fill out your copyright notice in the Description page of Project Settings.


#include "RotationRootMotionNotifyState.h"
#include "RotationRootMotionNotifyState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\LockOnOffComponent.h"

void URotationRootMotionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    AActor* ownerRef {MeshComp->GetOwner()};
    if (!IsValid(ownerRef)) {return;}

    UCharacterMovementComponent* movementComp {ownerRef->FindComponentByClass<UCharacterMovementComponent>()};
    if (!IsValid(movementComp)) {return;}

    ULockOnOffComponent* lockOnOffComp {ownerRef->FindComponentByClass<ULockOnOffComponent>()};

    //If the actor has the "LockOnOff" component and is locked on, don't allow rotation
    if (IsValid(lockOnOffComp) && lockOnOffComp->GetLockedOn()) {return;}

    movementComp->bAllowPhysicsRotationDuringAnimRootMotion = true;
}

void URotationRootMotionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    AActor* ownerRef {MeshComp->GetOwner()};
    if (!IsValid(ownerRef)) {return;}

    UCharacterMovementComponent* movementComp {ownerRef->FindComponentByClass<UCharacterMovementComponent>()};
    if (!IsValid(movementComp)) {return;}

    movementComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
}