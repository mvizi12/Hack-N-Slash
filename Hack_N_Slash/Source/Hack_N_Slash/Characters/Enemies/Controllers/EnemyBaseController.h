// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyBaseController.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API AEnemyBaseController : public AAIController
{
	GENERATED_BODY()

private:
	class UBehaviorTree* bTree;

	UPROPERTY(VisibleAnywhere)
	APawn* targetPawn; //The AI's target
	//APawn* selfPawn;
	class AEnemyBase* selfEnemyRef;

protected:
	UPROPERTY(EditAnywhere)
	bool bDebugMode {false};

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	UFUNCTION(BlueprintCallable)
	void HandleSensedSight(AActor* sensedActor);

	UFUNCTION(BlueprintCallable)
	void HandleSensedDamage(AActor* sensedActor);
	
public:
	virtual void Tick(float DeltaTime) override;
};
