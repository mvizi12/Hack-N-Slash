// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
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
EState AMainPlayer::GetState() const {return currentState;}

bool AMainPlayer::IsCurrentStateEqualToAny(TArray<EState> states) const
{
	return states.Contains(currentState);
}

bool AMainPlayer::IsInvincible() const {return bIsInvincible;}

void AMainPlayer::SetState(EState state) {currentState = state;}

void AMainPlayer::SetInvincible(bool invincible, bool indefinite, float duration = 0.0f)
{
	bIsInvincible = invincible;
	if (indefinite) {return;}

	FTimerHandle InvincibleTimerHandle;
	GetWorldTimerManager().SetTimer(InvincibleTimerHandle, this, &AMainPlayer::NegateInvincibility, duration, false);
}
/************************************Public Functions************************************/