// Fill out your copyright notice in the Description page of Project Settings.


#include "InvincibleNotifyState.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"

void UInvincibleNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    AActor* ownerRef {MeshComp->GetOwner()};
    if (!IsValid(ownerRef)) {return;}

    IFighter* iFighterRef {Cast<IFighter>(ownerRef)};
    if (iFighterRef == nullptr) {return;}

    iFighterRef->SetInvincibility(true, true, 0.0f);
}

void UInvincibleNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    AActor* ownerRef {MeshComp->GetOwner()};
    if (!IsValid(ownerRef)) {return;}

    IFighter* iFighterRef {Cast<IFighter>(ownerRef)};
    if (iFighterRef == nullptr) {return;}

    iFighterRef->SetInvincibility(false, true, 0.0f);
}