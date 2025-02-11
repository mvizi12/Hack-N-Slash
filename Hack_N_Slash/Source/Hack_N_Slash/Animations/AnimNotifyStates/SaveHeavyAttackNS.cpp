// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveHeavyAttackNS.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\CombatComponent.h"

void USaveHeavyAttackNS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    UCombatComponent* combatComp {MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>()};
	if (!IsValid(combatComp)) { return; }

    combatComp->SaveHeavyAttack();
}

void USaveHeavyAttackNS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    UCombatComponent* combatComp {MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>()};
	if (!IsValid(combatComp)) { return; }

    combatComp->SaveHeavyAttack();
}