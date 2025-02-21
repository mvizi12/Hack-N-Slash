// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Chase.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UBTT_Chase : public UBTTaskNode
{
	GENERATED_BODY()

private:
	class AEnemyBaseController* controllerRef;
	FScriptDelegate FinishChaseDelegate; //Special type for storing a function in a variable
	bool bIsFinished;

	void Abort(UBehaviorTreeComponent &, uint8 *);

protected:
	UPROPERTY(EditAnywhere, meta = (ToolTip = "Make sure this is less than the attack radius"))
	float acceptanceRadius {100.0f};

	virtual void TickTask(UBehaviorTreeComponent&, uint8*, float) override;

	UFUNCTION()
	void FinishChaseTask();

public:
	UBTT_Chase();
	virtual EBTNodeResult::Type	ExecuteTask(UBehaviorTreeComponent&, uint8*) override;
};
