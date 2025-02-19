// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\EState.h"
#include "PlayerActionsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnDodgeSignature, UPlayerActionsComponent, OnDodgeDelegate, float, cost);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnResetAttackBuffersSignature, UPlayerActionsComponent, OnResetAttackBuffersDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* characterRef;
	class UCharacterMovementComponent* movementComp;
	class ULockOnOffComponent* lockOnOffComp;
	class IMainPlayerI* iPlayerRef;
	class IFighter* iFighterRef;

	bool bSavedDodge {false};

	bool CanDodge();
	void PerformDodge();
	FRotator GetDesiredRotation();

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* dodgeMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* doubleJumpMontage;

	UPROPERTY(EditAnywhere)
	TArray<EState> dodgeCancelableStates {EState::Attack};

	UPROPERTY(EditAnywhere)
	float staminaCostDodge {5.0f};

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void Look(float yaw, float pitch);

	UFUNCTION(BlueprintCallable)
	void Move(float yaw, float pitch);

	UFUNCTION(BlueprintCallable)
	void ResetDodgeBuffer(); //For other to call via an event

public:
	UPROPERTY(BlueprintAssignable)
	FOnDodgeSignature OnDodgeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnResetAttackBuffersSignature OnResetAttackBuffersDelegate;

	UPlayerActionsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Dodge();

	void SaveDodge(); //Public so animations can call it
};
