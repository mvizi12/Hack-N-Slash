// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerActionsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACK_N_SLASH_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* characterRef;
	class IMainPlayerI* iPlayerRef;
	class IFighter* iFighterRef;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Jump();

public:	
	UPlayerActionsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
