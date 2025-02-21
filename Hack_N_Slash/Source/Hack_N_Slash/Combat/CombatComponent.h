// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\EState.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnAttackPerformedSignature, UCombatComponent, OnAttackPerformedDelegate, float, amount);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnResetDodgeBufferSignature, UCombatComponent, OnResetDodgeBufferDelegate);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnLaunchPlayerSignature, UCombatComponent, OnLaunchPlayerDelegate, FVector, distance);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnSmashAttackPerformedSignature, UCombatComponent, OnSmashAttackPerformedDelegate);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnRagePercentUpdateSignature, UCombatComponent, OnRagePercentUpdateDelegate, float, percent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* characterRef;
	class UCharacterMovementComponent* movementComp;
	USkeletalMeshComponent* skeletalMeshComp;
	class IMainPlayerI* iPlayerRef;
	class IFighter* iFighterRef;

	UParticleSystemComponent* rageModePSComp; //Component for rage mode particle system

	bool bSavedAerialDashAttack {false};
	bool bSavedLightAttack {false};
	bool bSavedHeavyAttack {false};
	bool bSavedRageMode {false};

	bool bCanAerialAttack {true}; //Flag to let the system know if aerial attacks are allowed
	bool bCanAerialDash {true}; //Flag to let the system know an aerial dash can be performed
	bool bCanResetAttack {false}; //Flag to let the system know is it can perform the ResetAttack function
	bool bCanSmashAttack {true}; //Flag to let the system know a smahs attack can be performed
	bool bComboStarter {false}; //Flag to let the system know a combo was performed
	bool bHeavyAttack {false}; //Flag to let the system know a heavy attack was performed
	bool bRageMode {false}; //Flag to let the system know wether the player is in rage mode or not
	bool bCanLoseRage {false}; // Flag to let the system know the player's rage value can be lost

	float yDir {0.0f}; //Vertical direction the player is holding on the left stick

	double rageDeprRate {1.0f};
	double maxRageVal {100.0f};

	void DeprecateRage();
	bool CanAttack() const;
	bool CanAerialAttack() const;
	bool CanEnterRageMode() const;
	bool CanSmashAttack() const;

	//UKismetMathLibrary::Wrap(comboCounter, -1, maxCombo - 1) should stop these 2 functions from returning nullptr
	UAnimMontage* GetComboExtenderAnimMontage();
	UAnimMontage* GetComboStarterAnimMontage();
	void PerformAttack(int); //Performs light or heavy attack
	void PerformComboExtender();
	void PerformComboStarter();
	void PerformLaunchAttack();
	void PerformSmashAttack();

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode;

	UPROPERTY(VisibleAnywhere)
	double currentRageVal {0.0f};

	UPROPERTY(EditDefaultsOnly)
	TArray<EState> attackCancelableStates {EState::Attack};

	UPROPERTY(EditDefaultsOnly)
	TArray<EState> invalidAttackStates {EState::Attack, EState::Death, EState::Dodge, EState::HitStun};
	
	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> lightMeleeMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> heavyMeleeMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> comboStarterMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> comboExtenderMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> aerialMeleeMontages;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* aerialDashMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* launchMeleeMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* smashMeleeMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* rageModeMontage;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* rageModeOverlay;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* rageModeVFX;

	UPROPERTY(VisibleAnywhere)
	int comboCounter {0};

	UPROPERTY(VisibleAnywhere)
	int comboStarterIndex {0}; //Used for getting the right combo extender montage

	UPROPERTY(EditDefaultsOnly)
	float lightMeleeStaminaCost {5.0f};

	UPROPERTY(EditDefaultsOnly)
	float heavyMeleeStaminaCost {8.0f};

	UPROPERTY(EditDefaultsOnly)
	float launchMeleeStaminaCost {5.0f};

	UPROPERTY(EditDefaultsOnly)
	float smashMeleeStaminaCost {10.0f};

	UPROPERTY(EditDefaultsOnly)
	float aerialMeleeStaminaCost {5.0f};

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void EnterRageMode();

	UFUNCTION(BlueprintPure)
	FVector GetSmashAttackDistance() const;

	UFUNCTION(BlueprintCallable)
	void IncreaseRageVal(double val);

	UFUNCTION(BlueprintCallable)
	void LaunchPlayer(FVector distance, float lerpSpeed);

	UFUNCTION(BlueprintCallable)
	void LightAttack(float y);

	UFUNCTION(BlueprintCallable)
	void HeavyAttack();

	UFUNCTION(BlueprintCallable)
	void ResetAttackBuffers();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttackPerformedSignature OnAttackPerformedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnResetDodgeBufferSignature OnResetDodgeBufferDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnLaunchPlayerSignature OnLaunchPlayerDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSmashAttackPerformedSignature OnSmashAttackPerformedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnRagePercentUpdateSignature OnRagePercentUpdateDelegate;

	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) //Public so animations can call it
	void HandleResetAttack();

	void AerialDashAttack();

	void ResetCombo(); //Public so animations can call it

	void SaveAerialDashAttack(); //Public so animations can call it
	
	void SaveLightAttack(); //Public so animations can call it

	void SaveHeavyAttack(); //Public so animations can call it

	void SaveRageMode(); //Public so animations can call it

	UFUNCTION(BlueprintCallable)
	void TryResetAttack();
};