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
	skeletalMeshComp = characterRef->GetComponentByClass<USkeletalMeshComponent>();
	movementComp = characterRef->GetCharacterMovement();
	iFighterRef = Cast<IFighter>(characterRef);
}

void UCombatEnemyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/************************************Private Functions************************************/
bool UCombatEnemyComponent::CanAttack() const {return !iFighterRef->IsCurrentStateEqualToAny(invalidAttackStates) && !movementComp->IsFalling();}
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
float UCombatEnemyComponent::GetAnimationDuration() const {return animDur;}

void UCombatEnemyComponent::PerformMeleeAttack()
{
	if (iFighterRef == nullptr || meleeMontages.IsEmpty() || !CanAttack()) {return;}
	int index = FMath::RandRange(0, meleeMontages.Num() - 1);
	iFighterRef->SetState(EState::Attack);
	animDur = characterRef->PlayAnimMontage(meleeMontages[index]);
}

void UCombatEnemyComponent::PerformRangedAttack()
{
	if (iFighterRef == nullptr || rangedMontages.IsEmpty() || !CanAttack()) {return;}
	int index = FMath::RandRange(0, rangedMontages.Num() - 1);
	iFighterRef->SetState(EState::Attack);
	animDur = characterRef->PlayAnimMontage(rangedMontages[index]);
}

void UCombatEnemyComponent::HandleResetAttack()
{
	//Ensures that if the player is in the air, we only reset their state when they hit the ground
	if (movementComp->MovementMode == MOVE_Flying)
	{
		movementComp->SetMovementMode(MOVE_Falling);
		bCanResetAttack = true;
	}
	else if (movementComp->MovementMode != MOVE_Falling) 
	{
		iFighterRef->SetState(EState::NoneState);
		animDur = 0.0f;
	}
}

void UCombatEnemyComponent::RotateToTarget(AActor* target, float interpSpeed)
{
	if (!IsValid(target)) {return;}

	FVector enemyLoc {characterRef->GetActorLocation()};
	FVector targetLoc {target->GetActorLocation()};
	FRotator enemyRot {characterRef->GetActorRotation()};
	FRotator desiredRot {UKismetMathLibrary::FindLookAtRotation(enemyLoc, targetLoc)};
	desiredRot.Roll = enemyRot.Roll;
	desiredRot.Pitch = enemyRot.Pitch;
	desiredRot = UKismetMathLibrary::RInterpTo_Constant(enemyRot, desiredRot, GetWorld()->GetDeltaSeconds(), interpSpeed);
	characterRef->SetActorRotation(desiredRot);
}

void UCombatEnemyComponent::SetAttackingOverlay(bool flag)
{
	if (skeletalMeshComp == nullptr) {return;}
	if (flag) {skeletalMeshComp->SetOverlayMaterial(attackingOverlay);}
	else {skeletalMeshComp->SetOverlayMaterial(nullptr);}
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