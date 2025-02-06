// Fill out your copyright notice in the Description page of Project Settings.
#include "RotateToTargetNotify.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\LockOnOffComponent.h"

void URotateToTargetNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    ULockOnOffComponent* lockOnOffComp {MeshComp->GetOwner()->FindComponentByClass<ULockOnOffComponent>()};
	if (!IsValid(lockOnOffComp)) { return; }

    lockOnOffComp->OnRotateToTargetDelegate.Broadcast();
}