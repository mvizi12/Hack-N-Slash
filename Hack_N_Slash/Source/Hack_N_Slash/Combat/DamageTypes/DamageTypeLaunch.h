// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTypeMain.h"
#include "DamageTypeLaunch.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UDamageTypeLaunch : public UDamageTypeMain
{
	GENERATED_BODY()

public:
	UDamageTypeLaunch()
	{
		damageType = EDamageType::Launch;
	}
};
