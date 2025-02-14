// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatEnemyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnLaunchEnemySignature, UCombatEnemyComponent, OnLaunchEnemyDelegate, FVector, distance);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnStopEnemyTimelinesSignature, UCombatEnemyComponent, OnStopEnemyTimelinesDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UCombatEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* characterRef;
	class UCharacterMovementComponent* movementComp;
	class IFighter* iFighterRef;

	bool bCanResetAttack {false}; //Flag to let the system know is it can perform the ResetAttack function

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> meleeMontages;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void LaunchEnemy(FVector distance, float lerpSpeed);

public:
	UPROPERTY(BlueprintAssignable)
	FOnLaunchEnemySignature OnLaunchEnemyDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStopEnemyTimelinesSignature OnStopEnemyTimelinesDelegate;

	UCombatEnemyComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) //Public so animations can call it
	void HandleResetAttack();

	UFUNCTION(BlueprintCallable)
	void TryResetAttack();
		
};
