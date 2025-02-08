// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnAttackPerformedSignature, UCombatComponent, OnAttackPerformedDelegate, float, amount);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnResetDodgeBufferSignature, UCombatComponent, OnResetDodgeBufferDelegate);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnLaunchCharSignature, UCombatComponent, OnLaunchCharDelegate, FVector, distance);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* characterRef;
	class UCharacterMovementComponent* movementComp;
	class IMainPlayerI* iPlayerRef;
	class IFighter* iFighterRef;

	bool bSavedLightAttack {false};
	bool bSavedHeavyAttack {false};

	bool bHeavyAttack {false}; //Flag to let the system know a heavy attack was performed
	bool bComboStarter {false}; //Flag to let the system know a combo was performed

	float yDir {0.0f}; //Vertical direction the player is holding on the left stick

	bool CanAttack();

	//UKismetMathLibrary::Wrap(comboCounter, -1, maxCombo - 1) should stop these 2 functions from returning nullptr
	UAnimMontage* GetComboExtenderAnimMontage();
	UAnimMontage* GetComboStarterAnimMontage();
	void PerformAttack(bool); //Performs light or heavy attack
	void PerformComboExtender();
	void PerformComboStarter();
	void PerformLaunchAttack();

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> lightMeleeMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> heavyMeleeMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> comboStarterMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> comboExtenderMontages;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* launchMeleeMontage;

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

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void LaunchChar(FVector distance, float interpSpeed);

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
	FOnLaunchCharSignature OnLaunchCharDelegate;

	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) //Public so animations can call it
	void HandleResetAttack();

	UFUNCTION(BlueprintCallable) //Public so animations can call it
	void ResetCombo();
	
	void SaveLightAttack(); //Public so animations can call it

	void SaveHeavyAttack(); //Public so animations can call it
};
