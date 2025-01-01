// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MainPlayerI.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMainPlayerI : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HACK_N_SLASH_API IMainPlayerI
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void EndLockOnWithActor(AActor*) {} //For when the locked on enemy dies
	virtual bool HasEnoughStamina(float) {return true;}
};
