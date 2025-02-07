// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UEnemyBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	//Unreal doesn't suggest private variables be exposed to blueprint
	private:
		APawn* enemyPawn;
		ACharacter* characterRef;
		class UCharacterMovementComponent* movementComp;

		void UpdateVelocity();

	protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector currentVeloctiy{0.0f, 0.0f, 0.0f};

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float currentSpeed {0.0f};

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsFalling {false};

		UFUNCTION(BlueprintCallable)
		void UpdateEnemyPawn();

		UFUNCTION(BlueprintCallable)
		void UpdateCurrentSpeed();

		UFUNCTION(BlueprintCallable)
		void UpdateIsFalling();
};
