// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\CombatEnemyComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\StatsComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\MainPlayerI.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	movementComp = GetCharacterMovement();
	combatEnemyComp = FindComponentByClass<UCombatEnemyComponent>();
	statsComp = FindComponentByClass<UStatsComponent>();
}

void AEnemyBase::DisableCollision() {if (currentState == EState::Death) {SetActorEnableCollision(false);}}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/************************************Private Functions************************************/
void AEnemyBase::NegateInvincibility() {bIsInvincible = !bIsInvincible;}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
EState AEnemyBase::GetState() const {return currentState;}

float AEnemyBase::GetStrength() const
{
	if (statsComp == nullptr) {return 0.0f;}
	return statsComp->stats[EStat::Strength];
}

void AEnemyBase::HandleDeath()
{
	/*******************Play death animation, stop the AI's brain, and disable collision************************/
	//blackBoardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::DeadE);
	SetState(EState::Death);
	float duration {0.0f};
	if (deathMontage != nullptr) {duration = PlayAnimMontage(deathMontage);}
	//controllerRef->GetBrainComponent()->StopLogic("Defeated");
	if (movementComp->MovementMode == MOVE_Flying) {movementComp->SetMovementMode(MOVE_Falling);}
	if ((movementComp->MovementMode != MOVE_Flying && movementComp->MovementMode != MOVE_Falling)) {SetActorEnableCollision(false);}

	/*******************Play death animation and stop the AI's brain************************/

	//FTimerHandle destroyTimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(destroyTimerHandle, this, &AEnemyBase::FinishDeathAnim, duration, false);
	
	/***************End the player's lock on to this enemy if they're locked on to them*******************/
	IMainPlayerI* iPlayerRef {GetWorld()->GetFirstPlayerController()->GetPawn<IMainPlayerI>()};
	if (!iPlayerRef) {return;}
	iPlayerRef->EndLockOnWithActor(this);
	/***************End the player's lock on to this enemy if they're locked on to them*******************/
}


bool AEnemyBase::IsCurrentStateEqualToAny(TArray<EState> states) const {return states.Contains(currentState);}

bool AEnemyBase::IsGrounded() const {return movementComp->IsMovingOnGround();}

bool AEnemyBase::IsInvincible() const {return bIsInvincible;}

void AEnemyBase::LaunchFighter(FVector distance)
{
	if (combatEnemyComp == nullptr) {return;}
	combatEnemyComp->OnLaunchEnemyDelegate.Broadcast(distance);
}

void AEnemyBase::ResumeKnockedDBMontage()
{
	if (statsComp == nullptr) {return;}
	statsComp->ResumeLoopedMontage();
}

void AEnemyBase::SetState(EState state)
{
	if (currentState == EState::Death || currentState == state) {return;}
	currentState = state;
}

void AEnemyBase::SetInvincibility(bool invincible, bool indefinite, float duration = 0.0f)
{
	bIsInvincible = invincible;
	if (indefinite) {return;}

	FTimerHandle InvincibleTimerHandle;
	GetWorldTimerManager().SetTimer(InvincibleTimerHandle, this, &AEnemyBase::NegateInvincibility, duration, false);
}

/************************************Public Functions************************************/