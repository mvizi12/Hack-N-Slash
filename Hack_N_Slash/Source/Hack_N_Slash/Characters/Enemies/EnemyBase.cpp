// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Characters\StatsComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	statsComp = FindComponentByClass<UStatsComponent>();
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

EState AEnemyBase::GetState() const {return currentState;}

float AEnemyBase::GetStrength() const
{
	if (statsComp == nullptr) {return 0.0f;}
	return statsComp->stats[EStat::Strength];
}

bool AEnemyBase::IsCurrentStateEqualToAny(TArray<EState> states) const {return states.Contains(currentState);}

bool AEnemyBase::IsInvincible() const {return bIsInvincible;}

void AEnemyBase::SetState(EState state) {currentState = state;}

void AEnemyBase::SetInvincibility(bool invincible, bool indefinite, float duration = 0.0f)
{
	bIsInvincible = invincible;
	if (indefinite) {return;}

	FTimerHandle InvincibleTimerHandle;
	GetWorldTimerManager().SetTimer(InvincibleTimerHandle, this, &AEnemyBase::NegateInvincibility, duration, false);
}

/************************************Private Functions************************************/
void AEnemyBase::NegateInvincibility()
{
	bIsInvincible = !bIsInvincible;
}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
/************************************Public Functions************************************/