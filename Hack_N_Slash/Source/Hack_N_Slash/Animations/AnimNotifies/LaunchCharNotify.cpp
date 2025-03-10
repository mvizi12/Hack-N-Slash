// Fill out your copyright notice in the Description page of Project Settings.
#include "LaunchCharNotify.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\CombatComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\CombatEnemyComponent.h"

void ULaunchCharNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    UCombatComponent* combatComp {MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>()};
	if (IsValid(combatComp)) {combatComp->OnLaunchPlayerDelegate.Broadcast(distance);}

    UCombatEnemyComponent* combatEnemyComp {MeshComp->GetOwner()->FindComponentByClass<UCombatEnemyComponent>()};
	if (IsValid(combatEnemyComp)) {combatEnemyComp->OnLaunchEnemyDelegate.Broadcast(distance);}
}