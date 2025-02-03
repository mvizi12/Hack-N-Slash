// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnOffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API ULockOnOffComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float dotProduct {FLT_MAX};

	TArray<AActor*> enemies; //Potential lock on targets
	//Making this a character instead of an actor because we want access to some functions in the "ACharacter" class
	ACharacter* ownerRef; //Reference to the owner of this component
	APlayerController* playerController;
	class UCharacterMovementComponent* characterMovementComponent;
	class USpringArmComponent* springArmComponent;
	class UCameraComponent* camComp;

	void FindActorsToLockOnTo();
	void GetReferences();
	float GetDistanceToTarget(FVector, FVector);
	void LockOn();
	void SetPlayerControlRotation();

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bLockedOn {false};

	UPROPERTY(EditAnywhere, meta = (ToolTip = "Added to the player's camera location when locke don"))
	FVector cameraOffset;

	UPROPERTY(EditAnywhere, meta = (ToolTip = "Positive number will tilt the camera up when locked on and negative number will tilt down"))
	float lockedOnCameraTiltZ {0.0f};

	UPROPERTY(EditAnywhere)
	float cameraInterpSpeed {30.0f};

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Radius around the player to trace for targets"))
	double traceRadius {1000.0f};

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ToggleLockOnOff();

public:
	AActor* currentTargetActor;

	ULockOnOffComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetLockedOn();		
	void LockOff();
};
