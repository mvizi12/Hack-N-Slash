// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WeaponTraceNotify.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UWeaponTraceNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Notify")
	float traceDistance;
	
	UPROPERTY(EditAnywhere, Category = "Notify")
	float traceRadius;
	
	UPROPERTY(EditAnywhere, Category = "Notify")
	float damage;
	
	virtual void Notify(USkeletalMeshComponent*, UAnimSequenceBase*, const FAnimNotifyEventReference&) override;
};
