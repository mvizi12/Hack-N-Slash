// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SaveLightAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API USaveLightAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent*, UAnimSequenceBase*, const FAnimNotifyEventReference&) override;
	
};
