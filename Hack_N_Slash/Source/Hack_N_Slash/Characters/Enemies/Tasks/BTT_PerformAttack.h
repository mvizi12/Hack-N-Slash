// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PerformAttack.generated.h"

/**
 * 
 */
UCLASS()
class HACK_N_SLASH_API UBTT_PerformAttack : public UBTTaskNode
{
	GENERATED_BODY()

private:
	class AEnemyBaseController* controllerRef;
	bool bIsFinished;

	void Abort(UBehaviorTreeComponent &, uint8 *);

protected:
	//UPROPERTY(EditAnywhere, meta = (ToolTip = "Make sure this is <= max attack radius"))
	//float acceptanceRadius {100.0f};

	UPROPERTY(EditAnywhere, meta = (Tooltip = "Is this attack ranged or not? EX: Shooting a porjectile"))
	bool bRanged {false};

	UPROPERTY(EditDefaultsOnly)
	UMaterial* attackingOverlay;

	virtual void TickTask(UBehaviorTreeComponent&, uint8*, float) override;

	UFUNCTION()
	void FinishAttackTask();

public:
	UBTT_PerformAttack();
	virtual EBTNodeResult::Type	ExecuteTask(UBehaviorTreeComponent&, uint8*) override;
};
