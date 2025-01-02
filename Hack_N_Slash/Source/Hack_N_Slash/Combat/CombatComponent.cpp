// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\MainPlayerI.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	characterRef = GetOwner<ACharacter>();
	iPlayerRef = Cast<IMainPlayerI>(characterRef);
	iFighterRef = Cast<IFighter>(characterRef);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/************************************Private Functions************************************/
bool UCombatComponent::CanAttack()
{
    TArray<EState> invalidAttackStates = {EState::Attack, EState::Death, EState::Dodge}; //States the player isn't allowed to attack
	return !iFighterRef->IsCurrentStateEqualToAny(invalidAttackStates);
}

/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void UCombatComponent::LightAttack()
{
	if (lightMeleeMontages.IsEmpty()) {return;}
	if (iFighterRef == nullptr || iPlayerRef == nullptr) {return;}
	if (!iPlayerRef->HasEnoughStamina(lightMeleeStaminaCost)) {return;}

	TArray<EState> states = {EState::Attack};
	if (iFighterRef->IsCurrentStateEqualToAny(states)) //If the fighter is currently attacking
	{
		//Save the input to buffer the next attack
		bSavedLightAttack = true;
	}
	else
	{
		//Else Attempt to attack
		if (CanAttack())
		{
			iFighterRef->SetState(EState::Attack);
			characterRef->PlayAnimMontage(lightMeleeMontages[comboCounter]);
			++comboCounter;
			int maxCombo {lightMeleeMontages.Num()};
			comboCounter = UKismetMathLibrary::Wrap(comboCounter, -1, maxCombo - 1);
			OnAttackPerformedDelegate.Broadcast(lightMeleeStaminaCost);
		}
	}
}
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
void UCombatComponent::HandleResetAttack()
{
	iFighterRef->SetState(EState::NoneState);
}

void UCombatComponent::ResetCombo()
{
	comboCounter = 0;
}
/************************************Public Functions************************************/