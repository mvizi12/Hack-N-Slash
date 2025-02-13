// Fill out your copyright notice in the Description page of Project Settings.
#include "CombatEnemyComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"

// Sets default values for this component's properties
UCombatEnemyComponent::UCombatEnemyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatEnemyComponent::BeginPlay()
{
	Super::BeginPlay();

	characterRef = GetOwner<ACharacter>();
	movementComp = characterRef->GetCharacterMovement();
	iFighterRef = Cast<IFighter>(characterRef);
}

void UCombatEnemyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/************************************Private Functions************************************/
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void UCombatEnemyComponent::LaunchEnemy(FVector distance, float lerpSpeed)
{
	FVector startLoc {characterRef->GetActorLocation()};
	FVector desiredLoc {startLoc + distance};
	FVector newLoc = UKismetMathLibrary::VLerp(startLoc, desiredLoc, lerpSpeed);
	characterRef->SetActorLocation(newLoc, true);
}
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
void UCombatEnemyComponent::HandleResetAttack()
{
	//Ensures that if the player is in the air, we only reset their state when they hit the ground
	if (movementComp->MovementMode == MOVE_Flying)
	{
		movementComp->SetMovementMode(MOVE_Falling);
		bCanResetAttack = true;
	}
	else if (movementComp->MovementMode != MOVE_Falling) {iFighterRef->SetState(EState::NoneState);}
}

void UCombatEnemyComponent::TryResetAttack()
{
	iFighterRef->ResumeKnockedDBMontage();
	if (!bCanResetAttack) {return;}
	bCanResetAttack = false;
	movementComp->SetMovementMode(MOVE_Walking);
	HandleResetAttack();
}
/************************************Public Functions************************************/