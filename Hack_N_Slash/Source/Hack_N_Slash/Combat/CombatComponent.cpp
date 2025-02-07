// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	movementComp = characterRef->GetCharacterMovement();
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
    TArray<EState> invalidAttackStates = {EState::Attack, EState::Death, EState::Dodge, EState::HitStun}; //States the player isn't allowed to attack
	return !iFighterRef->IsCurrentStateEqualToAny(invalidAttackStates) && !movementComp->IsFalling();
}

UAnimMontage *UCombatComponent::GetComboExtenderAnimMontage()
{
	if (comboStarterIndex >= comboExtenderMontages.Num()) {return nullptr;}
    return comboExtenderMontages[comboStarterIndex];
}

UAnimMontage* UCombatComponent::GetComboStarterAnimMontage()
{
	int temp = comboCounter - 1; //The 1st combo starter montage will correlate with the 1st heavy attack and so on
	if (temp >= comboStarterMontages.Num()) {return nullptr;}
    return comboStarterMontages[temp];
}

void UCombatComponent::PerformAttack(bool light)
{
	iFighterRef->SetState(EState::Attack);
	if (light)
	{
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Light Attack"));}
		characterRef->PlayAnimMontage(lightMeleeMontages[comboCounter]);
		++comboCounter;
		int maxCombo {lightMeleeMontages.Num()};
		comboCounter = UKismetMathLibrary::Wrap(comboCounter, -1, maxCombo - 1);
		OnAttackPerformedDelegate.Broadcast(lightMeleeStaminaCost);
	}
	else
	{
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Heavy Attack"));}
		characterRef->PlayAnimMontage(heavyMeleeMontages[comboCounter]);
		++comboCounter;
		int maxCombo {heavyMeleeMontages.Num()};
		comboCounter = UKismetMathLibrary::Wrap(comboCounter, -1, maxCombo - 1);
		OnAttackPerformedDelegate.Broadcast(heavyMeleeStaminaCost);
	}
}

void UCombatComponent::PerformComboExtender()
{
	bComboStarter = false;
	UAnimMontage* comboExtenderMontage = GetComboExtenderAnimMontage();
	if (comboExtenderMontage == nullptr)
	{
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Combo Extender Anim Montage Invalid"));}
		return;
	}
	iFighterRef->SetState(EState::Attack);
	characterRef->PlayAnimMontage(comboExtenderMontage);
	OnAttackPerformedDelegate.Broadcast(heavyMeleeStaminaCost);
}

void UCombatComponent::PerformComboStarter()
{
	bHeavyAttack = false;
	UAnimMontage* comboStarterMontage = GetComboStarterAnimMontage();
	if (comboStarterMontage == nullptr)
	{
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Combo Starter Anim Montage Invalid"));}
		return;
	}
	bComboStarter = true;
	comboStarterIndex = comboStarterMontages.IndexOfByKey(comboStarterMontage);
	iFighterRef->SetState(EState::Attack);
	characterRef->PlayAnimMontage(comboStarterMontage);
	OnAttackPerformedDelegate.Broadcast(lightMeleeStaminaCost);
}

void UCombatComponent::PerformLaunchAttack()
{
	if (launchMeleeMontage == nullptr) {return;}
	iFighterRef->SetState(EState::Attack);
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Launch Attack"));}
	characterRef->PlayAnimMontage(launchMeleeMontage);
	OnAttackPerformedDelegate.Broadcast(launchMeleeStaminaCost);
}

/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void UCombatComponent::LightAttack(float y)
{
	if (lightMeleeMontages.IsEmpty()) {return;}
	if (iFighterRef == nullptr || iPlayerRef == nullptr) {return;}
	if (!iPlayerRef->HasEnoughStamina(lightMeleeStaminaCost)) {return;}

	bSavedHeavyAttack = false; //Bookeeping since we're performing a light attack; necessary for chaining heavy with light attacks
	yDir = y;
	OnResetDodgeBufferDelegate.Broadcast();

	TArray<EState> states = {EState::Attack};
	if (iFighterRef->IsCurrentStateEqualToAny(states)) //If the fighter is currently attacking
	{
		//Save the input to buffer the next attack
		bSavedLightAttack = true;
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Saved Light Attack"));}
		return;
	}
	//Else Attempt to attack
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Attempting Light Attack"));}
	if (CanAttack())
	{
		if (yDir < 0) {PerformLaunchAttack();} //If player is holding back on left stick, perform launch attack
		else if (!bHeavyAttack) {PerformAttack(true);}
		else {PerformComboStarter();} //If a heavy attack was performed previosuly, this will be the start of a combo
	}
}

void UCombatComponent::HeavyAttack()
{
	if (heavyMeleeMontages.IsEmpty()) {return;}
	if (iFighterRef == nullptr || iPlayerRef == nullptr) {return;}
	if (!iPlayerRef->HasEnoughStamina(heavyMeleeStaminaCost)) {return;}

	bSavedLightAttack = false; //Bookeeping since we're performing a heavy attack; necessary for chaining light with heavy attacks
	OnResetDodgeBufferDelegate.Broadcast();
	bHeavyAttack = true;

	TArray<EState> states = {EState::Attack};
	if (iFighterRef->IsCurrentStateEqualToAny(states)) //If the fighter is currently attacking
	{
		//Save the input to buffer the next attack
		bSavedHeavyAttack = true;
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Saved Heavy Attack"));}
		return;
	}
	//Else Attempt to attack
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Attempting Heavy Attack"));}
	if (CanAttack())
	{
		if (!bComboStarter) {PerformAttack(false);}
		else {PerformComboExtender();}
	}
}

void UCombatComponent::ResetAttackBuffers()
{
	bSavedLightAttack = false;
	bSavedHeavyAttack = false;
}
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
void UCombatComponent::HandleResetAttack()
{
	iFighterRef->SetState(EState::NoneState); //Needed
	bSavedLightAttack = false;
	bSavedHeavyAttack = false;
	bHeavyAttack = false; //Extra insurance
	yDir = 0;
}

void UCombatComponent::ResetCombo()
{
	bComboStarter = false; //Extra insurance
	comboStarterIndex = 0; //Needed
	comboCounter = 0; //Needed
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
	LightAttack(yDir);
}

void UCombatComponent::SaveHeavyAttack()
{
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Calling SaveHeavyAttack"));}
	if (!bSavedHeavyAttack) {return;}
	bSavedHeavyAttack = false;

	//Decides wether or not the state should be reset
	//In large combat systems there's gonna be times when a variable is set even though you don't want it
	//So this serves as an extra layer of insurance
	TArray<EState> states = {EState::Attack};
	if (iFighterRef->IsCurrentStateEqualToAny(states)) {iFighterRef->SetState(EState::NoneState);}
	HeavyAttack();
}
/************************************Public Functions************************************/