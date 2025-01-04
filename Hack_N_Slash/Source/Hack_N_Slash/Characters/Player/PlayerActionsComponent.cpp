// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionsComponent.h"
#include "GameFramework/Character.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\MainPlayerI.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"

// Sets default values for this component's properties
UPlayerActionsComponent::UPlayerActionsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	characterRef = GetOwner<ACharacter>();
	iPlayerRef = Cast<IMainPlayerI>(characterRef);
	iFighterRef = Cast<IFighter>(characterRef);
}

void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/************************************Private Functions************************************/
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void UPlayerActionsComponent::Jump()
{
	if (iPlayerRef == nullptr || iFighterRef == nullptr) {return;}
	TArray<EState> states = {EState::NoneState};
	if (iFighterRef->IsCurrentStateEqualToAny(states)) {characterRef->Jump();}
	else {characterRef->StopJumping();}
}
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
/************************************Public Functions************************************/