// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnOffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API ULockOnOffComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bLockedOn {false};

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Positive number will tilt the camera up when locked on and negative number will tilt down"))
	float lockedOnCameraTiltZ {0.0f};

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Max distance the player can be from target to lock on"))
	double breakDistance {1000.0f};

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ToggleLockOnOff(float radius, FVector cameraOffset);

public:
	ULockOnOffComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetLockedOn();		
};
