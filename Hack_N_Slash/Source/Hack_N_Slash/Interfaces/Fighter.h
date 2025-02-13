// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\EState.h"
#include "Fighter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFighter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HACK_N_SLASH_API IFighter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EState GetState() const {return EState::NoneState;}
	virtual float GetStrength() const {return 0.0f;}
	virtual void HandleDeath() {}
	virtual bool IsCurrentStateEqualToAny(TArray<EState>) const {return false;}
	virtual bool IsGrounded() const {return true;}
	virtual bool IsInvincible() const {return false;}
	virtual void LaunchFighter(FVector) {}
	virtual void ResumeKnockedDBMontage() {}
	virtual void SetState(EState) {}
	virtual void SetInvincibility(bool, bool, float) {}
};
