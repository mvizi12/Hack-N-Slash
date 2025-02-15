// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	characterRef = GetOwner<ACharacter>();
	iFighterRef = Cast<IFighter>(characterRef);
	movementComp = characterRef->GetCharacterMovement();
}

void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RegenStamina();
}

/************************************Private Functions************************************/
UAnimMontage *UStatsComponent::GetHitReactionMontage(EDamageType damageType)
{
    switch (damageType)
	{
	case EDamageType::DefaultDMGType:
		UE_LOG(LogTemp, Warning, TEXT("Default"));
		return nullptr;
	case EDamageType::KnockBack:
		knockedBack = true;
		if (movementComp->IsFlying()) {movementComp->SetMovementMode(MOVE_Falling);}
		return kbHitMontage;
	case EDamageType::KnockDown:
		knockedDown = true;
		if (movementComp->IsFlying()) {movementComp->SetMovementMode(MOVE_Falling);}
		return kdHitMontage;
	case EDamageType::Launch:
		if (iFighterRef == nullptr) {return nullptr;}
		movementComp->SetMovementMode(EMovementMode::MOVE_Flying); //Character won't fall
		return launchMontage;
	case EDamageType::Left:
		if (movementComp->IsFlying() || movementComp->IsFalling()) {movementComp->SetMovementMode(MOVE_Flying);}
		return leftHitMontage;
	case EDamageType::Middle:
		if (movementComp->IsFlying() || movementComp->IsFalling()) {movementComp->SetMovementMode(MOVE_Flying);}
		return middleHitMontage;
	case EDamageType::Right:
		if (movementComp->IsFlying() || movementComp->IsFalling()) {movementComp->SetMovementMode(MOVE_Flying);}
		return rightHitMontage;
	default:
		return nullptr;
	}
}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void UStatsComponent::EnableStaminaRegen() {bCanRegenStamina = true;}

float UStatsComponent::GetStatPercentage(EStat current, EStat max) const
{return stats[current] / stats[max];}
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
void UStatsComponent::ReduceHealth(float damage, FVector buffer, AActor *opponent, EDamageType damageType)
{
	if (iFighterRef == nullptr) {return;}

	if (iFighterRef->IsInvincible()) {return;} //If fighter is invincible, return

	stats[EStat::Health] -= damage;
	stats[EStat::Health] = UKismetMathLibrary::FClamp(stats[EStat::Health], 0, stats[EStat::MaxHealth]);

	OnHealthPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Health, EStat::MaxHealth));

	if (stats[EStat::Health] <= 0) {OnZeroHealthUpdateDelegate.Broadcast();}
	else //Play hurt montage
	{
		UAnimMontage* hurtMontage = GetHitReactionMontage(damageType);
		iFighterRef->SetState(EState::HitStun);
		if (characterRef == nullptr || hurtMontage == nullptr) {return;}
		iFighterRef->LaunchFighter(buffer); //Launch the character in the specified direction
		characterRef->PlayAnimMontage(hurtMontage);
	}
}

void UStatsComponent::ReduceStamina(float amount)
{
	if (bIsEnemy) {return;}
	stats[EStat::Stamina] -= amount;
	stats[EStat::Stamina] = UKismetMathLibrary::FClamp(stats[EStat::Stamina], 0, stats[EStat::MaxStamina]);

	bCanRegenStamina = false;

	OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
	//0 - Because we don't need to worry about resuming this function if it's been interrupted
	//100 - A unique id
	FLatentActionInfo staminaRegenInfo {0, 100, TEXT("EnableStaminaRegen"), this};
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), staminaRegenDelay, staminaRegenInfo);
}

void UStatsComponent::RegenStamina()
{
	if (!bCanRegenStamina || bIsEnemy) {return;}

	stats[EStat::Stamina] = UKismetMathLibrary::FInterpTo_Constant(stats[EStat::Stamina],
	stats[EStat::MaxStamina], GetWorld()->DeltaTimeSeconds, staminaRegenRate);

	OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
}

void UStatsComponent::ResumeLoopedMontage()
{
	if (knockedDown)
	{
		movementComp->SetMovementMode(MOVE_Walking);
		characterRef->PlayAnimMontage(kdHitMontage, kdHitMontage->RateScale, TEXT("HitGround"));
		knockedDown = false;
	}
	/*else if (knockedBack)
	{
		movementComp->SetMovementMode(MOVE_Walking);
		characterRef->PlayAnimMontage(kbHitMontage, kbHitMontage->RateScale, TEXT("HitGround"));
		knockedBack = false;
	}*/

}
/************************************Public Functions************************************/