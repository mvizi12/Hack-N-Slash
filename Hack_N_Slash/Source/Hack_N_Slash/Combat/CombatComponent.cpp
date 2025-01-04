// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"
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

void UCombatComponent::PerformLightAttack()
{
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Light Attack"));}
	iFighterRef->SetState(EState::Attack);
	characterRef->PlayAnimMontage(lightMeleeMontages[comboCounter]);
	++comboCounter;
	int maxCombo {lightMeleeMontages.Num()};
	comboCounter = UKismetMathLibrary::Wrap(comboCounter, -1, maxCombo - 1);
	OnAttackPerformedDelegate.Broadcast(lightMeleeStaminaCost);
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
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Saved Light Attack"));}
	}
	else
	{
		//Else Attempt to attack
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Attempting Light Attack"));}
		if (CanAttack()) {PerformLightAttack();}
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

void UCombatComponent::SaveLightAttack()
{
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Calling SaveLightAttack"));}
	if (!bSavedLightAttack) {return;}
	bSavedLightAttack = false;

	//Decides wether or not the state should be reset
	//In large combat systems there's gonna be times when a variable is set even though you don't want it
	//So this serves as an extra layer of insurance
	TArray<EState> states = {EState::Attack};
	if (iFighterRef->IsCurrentStateEqualToAny(states)) {iFighterRef->SetState(EState::NoneState);}
	LightAttack();
}
/************************************Public Functions************************************/