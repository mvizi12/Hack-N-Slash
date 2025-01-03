// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\EState.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\MainPlayerI.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"
#include "MainPlayer.generated.h"

UCLASS()
class HACK_N_SLASH_API AMainPlayer : public ACharacter, public IMainPlayerI, public IFighter
{
	GENERATED_BODY()

private:
	void NegateInvincibility();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInvincible {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EState> currentState {EState::NoneState};
	
	virtual void BeginPlay() override;

public:
	AMainPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/***************Interface Functions - Fighter***************/
	virtual EState GetState() const override;
	virtual bool IsCurrentStateEqualToAny(TArray<EState>) const override;
	virtual bool IsInvincible() const override;
	virtual void SetState(EState) override;
	virtual void SetInvincible(bool, bool, float) override;

};
