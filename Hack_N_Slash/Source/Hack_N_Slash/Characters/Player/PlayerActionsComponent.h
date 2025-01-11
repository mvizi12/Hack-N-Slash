// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerActionsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnDodgeSignature, UPlayerActionsComponent, OnDodgeDelegate, float, cost);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* characterRef;
	class UCharacterMovementComponent* movementComp;
	class IMainPlayerI* iPlayerRef;
	class IFighter* iFighterRef;

	bool bSavedDodge {false};

	bool CanDodge();
	void PerformDodge();

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* dodgeMontage;

	UPROPERTY(EditAnywhere)
	float staminaCostDodge {5.0f};

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Dodge();

	UFUNCTION(BlueprintCallable)
	void Jump();

public:
	UPROPERTY(BlueprintAssignable)
	FOnDodgeSignature OnDodgeDelegate;

	UPlayerActionsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
