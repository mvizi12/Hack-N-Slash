// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	//Unreal doesn't suggest private variables be exposed to blueprint
	private:
		APawn* playerPawn;

		void UpdateVelocity();

	protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector currentVeloctiy{0.0f, 0.0f, 0.0f};

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float currentSpeed {0.0f};

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float currentAngle {0.0f};

		UFUNCTION(BlueprintCallable)
		void UpdatePlayerPawn();

		UFUNCTION(BlueprintCallable)
		void UpdateCurrentAngle();

		UFUNCTION(BlueprintCallable)
		void UpdateCurrentSpeed();
	
};
