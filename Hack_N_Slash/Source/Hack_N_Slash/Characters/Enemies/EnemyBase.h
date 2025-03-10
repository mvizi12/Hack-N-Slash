// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\EState.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Enemy.h"
#include "EnemyBase.generated.h"

UCLASS()
class HACK_N_SLASH_API AEnemyBase : public ACharacter, public IFighter, public IEnemy
{
	GENERATED_BODY()
	
private:
	class AEnemyBaseController* controllerRef;
	class UCharacterMovementComponent* movementComp;
	class UCombatEnemyComponent* combatEnemyComp;
	class UStatsComponent* statsComp;

	void NegateInvincibility();

protected:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* behaviorTree;

	UPROPERTY(EditAnywhere, meta = (Tooltip = "Make sure this is less than the enemy's sight radius"))
	float strafeDistance {200.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInvincible {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EState currentState {EState::NoneState};

	UPROPERTY(EditAnywhere)
	UAnimMontage* deathMontage;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void DisableCollision();

	UFUNCTION(BlueprintPure)
	AActor* GetCurrentTarget() const;

	UFUNCTION()
	void HandlePlayerDeath();

public:
	AEnemyBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;
	float GetStrafeDistance() const;

	/***************Interface Functions - Fighter***************/
	virtual float GetAnimationDuration() const;
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

	/***************Interface Functions - Enemy***************/
	virtual void Attack(bool) override;
	virtual void SetAttackingOverlay(bool flag) override;

};
