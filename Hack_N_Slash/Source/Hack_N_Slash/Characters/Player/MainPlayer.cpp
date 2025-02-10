// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
//#include "GameFramework/Character.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\CombatComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Combat\LockOnOffComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\StatsComponent.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	characterRef = GetOwner<ACharacter>();
	combatComp = FindComponentByClass<UCombatComponent>();
	lockOnOffComp = FindComponentByClass<ULockOnOffComponent>();
	statsComp = FindComponentByClass<UStatsComponent>();
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/************************************Private Functions************************************/
void AMainPlayer::NegateInvincibility()
{
	bIsInvincible = !bIsInvincible;
}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
void AMainPlayer::EndLockOnWithActor(AActor *actor)
{
	if (lockOnOffComp == nullptr) {return;}
	if (!IsValid(lockOnOffComp->currentTargetActor) || lockOnOffComp->currentTargetActor != actor) {return;}
	lockOnOffComp->LockOff();
}

EState AMainPlayer::GetState() const {return currentState;}

float AMainPlayer::GetStrength() const
{
	if (statsComp == nullptr) {return 0.0f;}
	return statsComp->stats[EStat::Strength];
}

void AMainPlayer::HandleDeath()
{
	SetState(EState::Death);
	DisableInput(GetController<APlayerController>());
	if (characterRef == nullptr) {return;}
	if (deathMontage != nullptr) {PlayAnimMontage(deathMontage);}
	characterRef->SetActorEnableCollision(false);
}

bool AMainPlayer::HasEnoughStamina(float staminaCost)
{
	if (statsComp == nullptr || !statsComp->stats[EStat::Stamina]) {return false;}
    return statsComp->stats[EStat::Stamina] >= staminaCost;
}

bool AMainPlayer::IsCurrentStateEqualToAny(TArray<EState> states) const
{
	return states.Contains(currentState);
}

bool AMainPlayer::IsInvincible() const {return bIsInvincible;}

void AMainPlayer::LaunchFighter(FVector distance)
{
	if (combatComp == nullptr) {return;}
	combatComp->OnLaunchPlayerDelegate.Broadcast(distance);
}

void AMainPlayer::SetState(EState state) {currentState = state;}

void AMainPlayer::SetInvincibility(bool invincible, bool indefinite, float duration = 0.0f)
{
	bIsInvincible = invincible;
	if (indefinite) {return;}

	FTimerHandle InvincibleTimerHandle;
	GetWorldTimerManager().SetTimer(InvincibleTimerHandle, this, &AMainPlayer::NegateInvincibility, duration, false);
}
/************************************Public Functions************************************/