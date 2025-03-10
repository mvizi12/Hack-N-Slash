// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
//#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\MainPlayerI.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\LockOnOffComponent.h"

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
	lockOnOffComp = characterRef->FindComponentByClass<ULockOnOffComponent>();
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
    TArray<EState> invalidDodgeStates = {EState::Attack, EState::Death, EState::Dodge, EState::HitStun}; //States the player isn't allowed to dodge in
	return !iFighterRef->IsCurrentStateEqualToAny(invalidDodgeStates) && !movementComp->IsFalling();
}

void UPlayerActionsComponent::PerformDodge()
{
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Dodge"));}
	iFighterRef->SetState(EState::Dodge);

	//Character will snap to the direction they're rotating in
	//EX: If the character is facing left, but the player holds right and tries to dodge, they'll snap and dodge to the right
	FRotator rotator = GetDesiredRotation();
	if (rotator != FRotator::ZeroRotator) {characterRef->SetActorRotation(rotator);}

	characterRef->PlayAnimMontage(dodgeMontage);
	OnDodgeDelegate.Broadcast(staminaCostDodge);
}

FRotator UPlayerActionsComponent::GetDesiredRotation()
{
	FVector lastInputVector = movementComp->GetLastInputVector();
	FRotator a = UKismetMathLibrary::MakeRotFromX(lastInputVector);
	bool bNotZeroVector = lastInputVector != FVector::ZeroVector;
	FRotator rotator = UKismetMathLibrary::SelectRotator(a, FRotator::ZeroRotator, bNotZeroVector);
    return rotator;
}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void UPlayerActionsComponent::Dodge()
{
	if (dodgeMontage == nullptr) {return;}
	if (iFighterRef == nullptr || iPlayerRef == nullptr) {return;}
	if (!iPlayerRef->HasEnoughStamina(staminaCostDodge)) {return;}

	OnResetAttackBuffersDelegate.Broadcast();

	if (iFighterRef->IsCurrentStateEqualToAny(dodgeCancelableStates))
	{
		bSavedDodge = true; //Saves the dodge input if trying to dodge while in any of the above states
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
	if (iPlayerRef == nullptr || iFighterRef == nullptr || movementComp == nullptr) {return;}
	TArray<EState> states = {EState::NoneState};
	if (!iFighterRef->IsCurrentStateEqualToAny(states)) //If character isn't in the "None" state, stop jumping
	{
		characterRef->StopJumping();
		return;
	}

	characterRef->Jump();
	//Perform Double Jump animation if already in the air and the character still has jumps left
	if (movementComp->IsFalling() && characterRef->JumpCurrentCount < characterRef->JumpMaxCount) {characterRef->PlayAnimMontage(doubleJumpMontage);}
}

void UPlayerActionsComponent::Look(float yaw, float pitch)
{
	//If player is locked on to an enemy, try targetting another enemy
	//Else do regular look input
	if (lockOnOffComp->GetLockedOn()) {lockOnOffComp->FindActorsToLockOnTo(yaw);}
	else
	{
		characterRef->AddControllerYawInput(yaw);
		characterRef->AddControllerPitchInput(pitch);
	}
}

void UPlayerActionsComponent::Move(float yaw, float pitch)
{
	EState currentState = iFighterRef->GetState();
	if (movementComp->IsFlying() || currentState == EState::HitStun || currentState == EState::Death) {return;}

	FRotator controlRotA {characterRef->GetControlRotation()};
	controlRotA.Pitch = 0.0;
	characterRef->AddMovementInput(UKismetMathLibrary::GetRightVector(controlRotA), yaw);

	FRotator controlRotB {characterRef->GetControlRotation()};
	controlRotB.Roll = 0.0f;
	controlRotB.Pitch = 0.0f;
	characterRef->AddMovementInput(UKismetMathLibrary::GetForwardVector(controlRotB), pitch);
}

void UPlayerActionsComponent::ResetDodgeBuffer() {bSavedDodge = false;}
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
void UPlayerActionsComponent::SaveDodge()
{
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Calling SaveDodge"));}
	if (!bSavedDodge) {return;}
	bSavedDodge = false;

	//Decides wether or not the state should be reset
	//In large combat systems there's gonna be times when a variable is set even though you don't want it
	//So this serves as an extra layer of insurance
	if (iFighterRef->IsCurrentStateEqualToAny(dodgeCancelableStates)) {iFighterRef->SetState(EState::NoneState);}
	Dodge();
}
/************************************Public Functions************************************/