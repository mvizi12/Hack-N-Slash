// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveDodgeNS.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\Player\PlayerActionsComponent.h"

void USaveDodgeNS::NotifyTick(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, float FrameDeltaTime, const FAnimNotifyEventReference &EventReference)
{
    UPlayerActionsComponent* playerActionsComp {MeshComp->GetOwner()->FindComponentByClass<UPlayerActionsComponent>()};
	if (!IsValid(playerActionsComp)) { return; }

    playerActionsComp->SaveDodge();
}