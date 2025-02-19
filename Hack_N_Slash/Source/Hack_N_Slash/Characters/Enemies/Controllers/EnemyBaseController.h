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
	class UBlackboardComponent* blackBoardComp;

	UPROPERTY(VisibleAnywhere)
	APawn* targetPawn; //The AI's target
	//APawn* selfPawn;
	class AEnemyBase* selfEnemyRef;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;
	
public:
	virtual void Tick(float DeltaTime) override;
};
