// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EEnemyState
{
	IdleE UMETA(DisplayName = "Idle"),
	ChaseE UMETA(DisplayName = "Chase"),
	StrafeE UMETA(DisplayName = "Strafe"),
	AttackE UMETA(DisplayName = "Attack"),
	DisabledE UMETA(DisplayName = "Disabled"),
	DeadE UMETA(DisplayName = "Dead"),
};