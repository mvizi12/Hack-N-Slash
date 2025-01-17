// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\DamageTypes\DamageTypeMain.h"
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
	TSubclassOf<UDamageTypeMain> damageType;

	UPROPERTY(EditAnywhere, Category = "Notify")
	float traceDistance;
	
	UPROPERTY(EditAnywhere, Category = "Notify")
	float traceRadius;
	
	UPROPERTY(EditAnywhere, Category = "Notify")
	float damage;
	
	virtual void Notify(USkeletalMeshComponent*, UAnimSequenceBase*, const FAnimNotifyEventReference&) override;
};
