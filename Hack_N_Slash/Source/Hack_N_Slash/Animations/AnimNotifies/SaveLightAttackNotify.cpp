// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveLightAttackNotify.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\CombatComponent.h"

void USaveLightAttackNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    UCombatComponent* combatComp {MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>()};
	if (!IsValid(combatComp)) { return; }

    combatComp->SaveLightAttack();
}
