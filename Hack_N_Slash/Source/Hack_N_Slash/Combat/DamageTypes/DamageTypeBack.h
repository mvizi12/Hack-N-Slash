// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTypeMain.h"
#include "DamageTypeBack.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UDamageTypeBack : public UDamageTypeMain
{
	GENERATED_BODY()

public:
	UDamageTypeBack()
	{
		damageType = EDamageType::Back;
	}
};
