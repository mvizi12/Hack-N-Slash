// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
 enum EDamageType
 {
	NoneDMGType UMETA(DisplayName = "None"),
	Back UMETA(DisplayName = "Back"),
	KnockBack UMETA(DisplayName = "Knockback"),
	KnockDown UMETA(DisplayName = "Knockdown"),
	Left UMETA(DisplayName = "Left"),
	Middle UMETA(DisplayName = "Middle"),
	Right UMETA(DisplayName = Right)
 };
