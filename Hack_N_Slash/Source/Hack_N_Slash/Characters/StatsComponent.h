// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EStat.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnHealthPercentUpdateSignature, UStatsComponent, OnHealthPercentUpdateDelegate, float, newPercent);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnStaminaPercentUpdateSignature, UStatsComponent, OnStaminaPercentUpdateDelegate, float, newPercent);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnZeroHealthUpdateSignature, UStatsComponent, OnZeroHealthUpdateDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;\

	UPROPERTY(VisibleAnywhere)
	bool bCanRegenStamina {true};

	UPROPERTY(EditAnywhere)
	float staminaRegenDelay {2.0f};

	UPROPERTY(EditAnywhere)
	double staminaRegenRate {10.0};

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
	void ReduceHealth(float damage, AActor* opponent);

	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float amount);

	UFUNCTION(BlueprintCallable)
	void RegenStamina();
};
