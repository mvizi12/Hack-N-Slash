// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTypeMain.h"
#include "DamageTypeLeft.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UDamageTypeLeft : public UDamageTypeMain
{
	GENERATED_BODY()

public:
	UDamageTypeLeft()
	{
		damageType = EDamageType::Left;
	}
};
