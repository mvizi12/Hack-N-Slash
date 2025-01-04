// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnAttackPerformedSignature, UCombatComponent, OnAttackPerformedDelegate, float, amount);


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

	bool CanAttack();
	void PerformAttack(bool);

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> lightMeleeMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> heavyMeleeMontages;

	UPROPERTY(VisibleAnywhere)
	int comboCounter {0};

	UPROPERTY(EditDefaultsOnly)
	float lightMeleeStaminaCost {5.0f};

	UPROPERTY(EditDefaultsOnly)
	float heavyMeleeStaminaCost {8.0f};

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void LightAttack();

	UFUNCTION(BlueprintCallable)
	void HeavyAttack();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttackPerformedSignature OnAttackPerformedDelegate;

	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) //Public so animations can call it
	void HandleResetAttack();

	UFUNCTION(BlueprintCallable) //Public so animations can call it
	void ResetCombo();
	
	void SaveLightAttack();

	void SaveHeavyAttack();
};
