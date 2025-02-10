// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\EDamageType.h"
#include "EStat.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnHealthPercentUpdateSignature, UStatsComponent, OnHealthPercentUpdateDelegate, float, newPercent);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnStaminaPercentUpdateSignature, UStatsComponent, OnStaminaPercentUpdateDelegate, float, newPercent);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnZeroHealthUpdateSignature, UStatsComponent, OnZeroHealthUpdateDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* characterRef;
	class IFighter* iFighterRef;
	class UCharacterMovementComponent* movementComp;

	UAnimMontage* GetHitReactionMontage(EDamageType) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	bool bIsEnemy {false};

	UPROPERTY(VisibleAnywhere)
	bool bCanRegenStamina {true};

	UPROPERTY(EditAnywhere)
	float staminaRegenDelay {2.0f};

	UPROPERTY(EditAnywhere)
	double staminaRegenRate {10.0};

	UPROPERTY(EditAnywhere)
	UAnimMontage* deathMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* kbHitMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* kdHitMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* launchMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* leftHitMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* middleHitMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* rightHitMontage;

	UFUNCTION()
	void EnableStaminaRegen();

	UFUNCTION(BlueprintPure)
	float GetStatPercentage(EStat current, EStat max) const;

public:
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EStat>, float> stats;

	UPROPERTY(BlueprintAssignable)
	FOnHealthPercentUpdateSignature OnHealthPercentUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaPercentUpdateSignature OnStaminaPercentUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthUpdateSignature OnZeroHealthUpdateDelegate;

	UStatsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float damage, AActor* opponent, EDamageType damageType);

	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float amount);

	UFUNCTION(BlueprintCallable)
	void RegenStamina();
};
