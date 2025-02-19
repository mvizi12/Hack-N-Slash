// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SaveHeavyAttackNS.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API USaveHeavyAttackNS : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent*, UAnimSequenceBase*, float, const FAnimNotifyEventReference&) override;
};
