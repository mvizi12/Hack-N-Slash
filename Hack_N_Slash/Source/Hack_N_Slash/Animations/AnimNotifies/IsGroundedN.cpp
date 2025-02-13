// Fill out your copyright notice in the Description page of Project Settings.


#include "IsGroundedN.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\CombatComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\CombatEnemyComponent.h"

void UIsGroundedN::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
	IFighter* iFighterRef {Cast<IFighter>(MeshComp->GetOwner())};
    if (iFighterRef == nullptr || !iFighterRef->IsGrounded()) {return;}

    UCombatComponent* combatComp {MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>()};
	if (IsValid(combatComp)) {combatComp->TryResetAttack();}

    UCombatEnemyComponent* combatEnemyComp {MeshComp->GetOwner()->FindComponentByClass<UCombatEnemyComponent>()};
	if (IsValid(combatEnemyComp)) {combatEnemyComp->TryResetAttack();}
}