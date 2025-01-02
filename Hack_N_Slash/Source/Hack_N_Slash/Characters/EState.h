// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
 enum EState
 {
	NoneState UMETA(DisplayName = "None"),
	Attack UMETA(DisplayName = "Attack"),
	Death UMETA(DisplayName = "Death"),
	Dodge UMETA(DisplayName = "Dodge")
 };