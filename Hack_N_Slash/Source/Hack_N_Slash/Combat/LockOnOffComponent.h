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
	TArray<FHitResult> enemyHits; //Potential lock on targets
	//Making this a character instead of an actor because we want access to some functions in the "ACharacter" class
	ACharacter* ownerRef; //Reference to the owner of this component
	APlayerController* playerController;
	class UCharacterMovementComponent* characterMovementComponent;
	class USpringArmComponent* springArmComponent;
	class UCameraComponent* camComp;

	void GetReferences();
	void LockOn(float, FVector);

protected:
	UPROPERTY(VisibleAnywhere)
	bool bDebugMode {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bLockedOn {false};

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Positive number will tilt the camera up when locked on and negative number will tilt down"))
	float lockedOnCameraTiltZ {0.0f};

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Max distance the player can be from target to lock on"))
	double traceDistance {1000.0f};

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ToggleLockOnOff(float traceRadius, FVector cameraOffset);

public:
	AActor* currentTargetActor;

	ULockOnOffComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetLockedOn();		
	void LockOff();
};
