// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "InvincibleNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UInvincibleNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent*, UAnimSequenceBase*, float, const FAnimNotifyEventReference&) override;
	virtual void NotifyEnd(USkeletalMeshComponent*, UAnimSequenceBase*, const FAnimNotifyEventReference&) override;
};
