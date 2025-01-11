// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
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
	movementComp = characterRef->GetCharacterMovement();
	iPlayerRef = Cast<IMainPlayerI>(characterRef);
	iFighterRef = Cast<IFighter>(characterRef);
}

void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/************************************Private Functions************************************/
bool UPlayerActionsComponent::CanDodge()
{
    TArray<EState> invalidDodgeStates = {EState::Attack, EState::Death, EState::Dodge}; //States the player isn't allowed to attack
	return !iFighterRef->IsCurrentStateEqualToAny(invalidDodgeStates) && !movementComp->IsFalling();
}

void UPlayerActionsComponent::PerformDodge()
{
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Dodge"));}
	characterRef->PlayAnimMontage(dodgeMontage);
	OnDodgeDelegate.Broadcast(staminaCostDodge);
}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void UPlayerActionsComponent::Dodge()
{
	if (dodgeMontage == nullptr) {return;}
	if (iFighterRef == nullptr || iPlayerRef == nullptr) {return;}
	if (!iPlayerRef->HasEnoughStamina(staminaCostDodge)) {return;}

	//TArray<EState> states = {EState::Attack, EState::Dodge}; //Can cancel attacks and dodges
	TArray<EState> states = {EState::Attack};
	if (iFighterRef->IsCurrentStateEqualToAny(states))
	{
		bSavedDodge = true;
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Saved Dodge"));}
	}
	else
	{
		//Else Attempt dodge
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Attempting Light Attack"));}
		if (CanDodge()) {PerformDodge();}
	}
}

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