// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageTypes\DamageTypeMain.h"
#include "TraceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FOnReportDamageSignature, UTraceComponent, OnReportDamageDelegate, AActor*, damagedActor, float, damage, FVector, eventLocation);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	AActor* ownerRef; //Reference to the owner of this component
	TArray<AActor*> actorsToIgnore;

	void GetReferences();
	void HandleDamage(TSubclassOf<UDamageTypeMain>, TArray<FHitResult>&, float);

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode;

	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnReportDamageSignature OnReportDamageDelegate;

	UTraceComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack(); //Public so animations can call it

	void WeaponTrace(TSubclassOf<UDamageTypeMain>, float, float, float); //Public so animations can call it
};
