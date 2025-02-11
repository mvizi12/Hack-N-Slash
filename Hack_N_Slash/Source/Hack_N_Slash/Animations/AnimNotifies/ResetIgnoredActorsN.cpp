// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetIgnoredActorsN.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\TraceComponent.h"

void UResetIgnoredActorsN::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    UTraceComponent* traceComp {MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>()};
	if (!IsValid(traceComp)) {return;}

    traceComp->HandleResetAttack();
}