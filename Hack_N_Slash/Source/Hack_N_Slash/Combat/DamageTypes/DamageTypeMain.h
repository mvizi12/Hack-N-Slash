// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\EDamageType.h"
#include "DamageTypeMain.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UDamageTypeMain : public UDamageType
{
	GENERATED_BODY()

public:
	TEnumAsByte<EDamageType> damageType {EDamageType::NoneDMGType};
	
};