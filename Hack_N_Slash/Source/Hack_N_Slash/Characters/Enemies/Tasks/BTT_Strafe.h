// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Strafe.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UBTT_Strafe : public UBTTaskNode
{
	GENERATED_BODY()

private:
	class AEnemyBaseController* controllerRef;
	FScriptDelegate FinishStrafeDelegate; //Special type for storing a function in a variable
	bool bIsFinished;

	void Abort(UBehaviorTreeComponent &, uint8 *);

protected:
	UPROPERTY(EditAnywhere, meta = (ToolTip = "Make sure this is <= max attack radius"))
	float acceptanceRadius {100.0f};

	UPROPERTY(EditAnywhere, meta = (Tooltip = "Higher number is higher amount of strafing time before attacking"))
	double threshold {0.9};

	virtual void TickTask(UBehaviorTreeComponent&, uint8*, float) override;

	UFUNCTION()
	void FinishStrafeTask();

public:
	UBTT_Strafe();
	virtual EBTNodeResult::Type	ExecuteTask(UBehaviorTreeComponent&, uint8*) override;
};
