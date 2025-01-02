// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
 enum EStat
 {
	NoneStat UMETA(DisplayName = "None"),
	Health UMETA(DisplayName = "Current Health"),
	MaxHealth UMETA(DisplayName = "Max Health"),
	Stamina UMETA(DisplayName = "Current Stamina"),
	MaxStamina UMETA(DisplayName = "Max Stamina"),
	Strength UMETA(DisplayName = "Strength")
 };