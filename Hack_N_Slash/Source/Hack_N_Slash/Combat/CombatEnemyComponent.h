// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\EState.h"
#include "CombatEnemyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnLaunchEnemySignature, UCombatEnemyComponent, OnLaunchEnemyDelegate, FVector, distance);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnRotateToTargetESignature, UCombatEnemyComponent, OnRotateToTargetEDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UCombatEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* characterRef;
	USkeletalMeshComponent* skeletalMeshComp;
	class UCharacterMovementComponent* movementComp;
	class IFighter* iFighterRef;
	bool bCanResetAttack {false}; //Flag to let the system know is it can perform the ResetAttack function
	float animDur {0.0f}; //For the behavior tree

	bool CanAttack() const;

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* attackingOverlay;

	UPROPERTY(EditDefaultsOnly)
	TArray<EState> invalidAttackStates {EState::Attack, EState::Death, EState::Dodge, EState::HitStun};

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> meleeMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> rangedMontages;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void LaunchEnemy(FVector distance, float lerpSpeed);

	UFUNCTION(BlueprintCallable)
	void RotateToTarget(AActor* target, float interpSpeed);

public:
	UPROPERTY(BlueprintAssignable)
	FOnLaunchEnemySignature OnLaunchEnemyDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnRotateToTargetESignature OnRotateToTargetEDelegate;

	UCombatEnemyComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetAnimationDuration() const;
	void PerformMeleeAttack();
	void PerformRangedAttack();

	UFUNCTION(BlueprintCallable) //Public so animations can call it
	void HandleResetAttack();

	void SetAttackingOverlay(bool);

	UFUNCTION(BlueprintCallable)
	void TryResetAttack();
		
};
