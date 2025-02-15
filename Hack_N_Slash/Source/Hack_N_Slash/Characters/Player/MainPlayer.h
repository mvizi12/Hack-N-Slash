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
	class UCharacterMovementComponent* movementComp;
	class UCombatComponent* combatComp;
	class ULockOnOffComponent* lockOnOffComp;
	class UStatsComponent* statsComp;

	void NegateInvincibility();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInvincible {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EState currentState {EState::NoneState};

	UPROPERTY(EditAnywhere)
	UAnimMontage* deathMontage;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void DisableCollision();

public:
	AMainPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/***************Interface Functions - Main Player***************/
	virtual void EndLockOnWithActor(AActor*) override; //For when the locked on enemy dies
	virtual bool HasEnoughStamina(float) override;

	/***************Interface Functions - Fighter***************/
	virtual EState GetState() const override;
	virtual float GetStrength() const override;
	UFUNCTION(BlueprintCallable)
	virtual void HandleDeath() override;
	virtual bool IsCurrentStateEqualToAny(TArray<EState>) const override;
	virtual bool IsGrounded() const override;
	virtual bool IsInvincible() const override;
	virtual void LaunchFighter(FVector) override;
	//Continues after the looped section of the knocked down or back animation montage
	virtual void ResumeKnockedDBMontage() override;
	virtual void SetState(EState) override;
	virtual void SetInvincibility(bool, bool, float) override;

};
