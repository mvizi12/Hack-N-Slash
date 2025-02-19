// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
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
	skeletalMeshComp = characterRef->GetComponentByClass<USkeletalMeshComponent>();
	iPlayerRef = Cast<IMainPlayerI>(characterRef);
	iFighterRef = Cast<IFighter>(characterRef);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DeprecateRage();
}

/************************************Private Functions************************************/
bool UCombatComponent::CanAttack() const {return !iFighterRef->IsCurrentStateEqualToAny(invalidAttackStates) && !movementComp->IsFalling();}

bool UCombatComponent::CanAerialAttack() const {return bCanAerialAttack && movementComp->MovementMode == MOVE_Flying;}

bool UCombatComponent::CanEnterRageMode() const {return !iFighterRef->IsCurrentStateEqualToAny(invalidAttackStates) && !bRageMode && currentRageVal >= maxRageVal && movementComp->MovementMode != MOVE_Flying && !movementComp->IsFalling();}

bool UCombatComponent::CanSmashAttack() const {return bCanSmashAttack && movementComp->MovementMode == MOVE_Flying;}

void UCombatComponent::DeprecateRage()
{
	if (!bCanLoseRage) {return;}

	currentRageVal = UKismetMathLibrary::FInterpTo_Constant(currentRageVal, 0.0f, GetWorld()->DeltaTimeSeconds, rageDeprRate);
	if (currentRageVal <= 0.0f)
	{
		bRageMode = false;
		bCanLoseRage = false;
		if (skeletalMeshComp) {skeletalMeshComp->SetOverlayMaterial(nullptr);}
	}

	//OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
}

UAnimMontage *UCombatComponent::GetComboExtenderAnimMontage()
{
	if (comboStarterIndex >= comboExtenderMontages.Num() || comboStarterIndex < 0) {return nullptr;}
    return comboExtenderMontages[comboStarterIndex];
}

UAnimMontage* UCombatComponent::GetComboStarterAnimMontage()
{
	int temp = comboCounter - 1; //The 1st combo starter montage will correlate with the 1st heavy attack and so on
	if (temp >= comboStarterMontages.Num() || temp < 0) {return nullptr;}
    return comboStarterMontages[temp];
}

void UCombatComponent::PerformAttack(int flag)
{
	iFighterRef->SetState(EState::Attack);
	switch (flag)
	{
	case 1: //Light attack
	{
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Light Attack"));}
		characterRef->PlayAnimMontage(lightMeleeMontages[comboCounter]);
		++comboCounter;
		int maxCombo {lightMeleeMontages.Num()};
		comboCounter = UKismetMathLibrary::Wrap(comboCounter, -1, maxCombo - 1);
		OnAttackPerformedDelegate.Broadcast(lightMeleeStaminaCost);
		break;
	}
	case 2: //Heavy Attack
	{
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Heavy Attack"));}
		characterRef->PlayAnimMontage(heavyMeleeMontages[comboCounter]);
		++comboCounter;
		int maxCombo {heavyMeleeMontages.Num()};
		comboCounter = UKismetMathLibrary::Wrap(comboCounter, -1, maxCombo - 1);
		OnAttackPerformedDelegate.Broadcast(heavyMeleeStaminaCost);
		break;
	}
	case 3: //Aerial Attack
	{
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Aerial Attack"));}
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Combo Counter: %d"), comboCounter));}
		characterRef->PlayAnimMontage(aerialMeleeMontages[comboCounter]);
		++comboCounter;
		int maxCombo {aerialMeleeMontages.Num()};
		comboCounter = UKismetMathLibrary::Wrap(comboCounter, -1, maxCombo - 1);
		if (comboCounter == 0) {bCanAerialAttack = false;} //Restricts aerial combat to at most, 1 full combo
		OnAttackPerformedDelegate.Broadcast(aerialMeleeStaminaCost);
		break;
	}
	default:
		if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Default switch"));}
		break;
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
	ResetCombo();
	characterRef->PlayAnimMontage(launchMeleeMontage);
	movementComp->SetMovementMode(MOVE_Flying); //Player won't fall
	OnAttackPerformedDelegate.Broadcast(launchMeleeStaminaCost);
}

void UCombatComponent::PerformSmashAttack()
{
	if (smashMeleeMontage == nullptr) {return;}
	iFighterRef->SetState(EState::Attack);
	if (GEngine && bDebugMode) {GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Performing Smash Attack"));}
	bCanAerialAttack = false;
	bCanSmashAttack = false;
	characterRef->PlayAnimMontage(smashMeleeMontage);
	movementComp->SetMovementMode(MOVE_Falling);
	OnAttackPerformedDelegate.Broadcast(smashMeleeStaminaCost);
}

/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void UCombatComponent::EnterRageMode()
{
	if (CanEnterRageMode())
	{
		iFighterRef->SetState(EState::Attack);
		if (skeletalMeshComp) {skeletalMeshComp->SetOverlayMaterial(rageModeOverlay);}
		characterRef->PlayAnimMontage(rageModeMontage);
		bRageMode = true;
	}
	else if (iFighterRef->IsCurrentStateEqualToAny(attackCancelableStates)) {bSavedRageMode = true;}
}

FVector UCombatComponent::GetSmashAttackDistance() const
{
	FHitResult outHit;
	FVector startLoc {characterRef->GetActorLocation()};
	FVector endLoc {0.0f, 0.0f, -1000000.0f};
	FCollisionObjectQueryParams objectParams;
	objectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	objectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	FCollisionQueryParams ignoreParams {FName {TEXT("Ignore Collision Parameters")}, false, characterRef};

	bool bHit = GetWorld()->LineTraceSingleByObjectType(outHit, startLoc, endLoc, objectParams, ignoreParams);
	if (bDebugMode) {DrawDebugLine(GetWorld(), startLoc, outHit.Location, bHit ? FColor::Green : FColor::Red, false, 1.0f, 0, 1.0f);}
	if (bHit) {UE_LOG(LogTemp, Warning, TEXT("Object blocking sight: %s"), *outHit.GetActor()->GetName());}

	if (bHit)
	{
		double zDistance {startLoc.Z - outHit.Location.Z};
		zDistance *= -1.0f;
		return {0.0f, 0.0f, zDistance};
	}
	else {return {0.0f, 0.0f, -100.0f};}
}

void UCombatComponent::IncreaseRageVal(double val)
{
	if (bRageMode) {return;}
	currentRageVal += val;
	currentRageVal = UKismetMathLibrary::FClamp(currentRageVal, 0, maxRageVal);
}

void UCombatComponent::LaunchPlayer(FVector distance, float lerpSpeed)
{
	FVector startLoc {characterRef->GetActorLocation()};
	FVector desiredLoc {startLoc + distance};
	FVector newLoc = UKismetMathLibrary::VLerp(startLoc, desiredLoc, lerpSpeed);
	characterRef->SetActorLocation(newLoc, true);
}

void UCombatComponent::LightAttack(float y)
{
	if (lightMeleeMontages.IsEmpty()) {return;}
	if (iFighterRef == nullptr || iPlayerRef == nullptr) {return;}
	if (!bRageMode) //If player isn't in rage mode, they need to have enough stamina
	{
		if (!iPlayerRef->HasEnoughStamina(lightMeleeStaminaCost)) {return;}
	}

	bSavedHeavyAttack = false; //Necessary for chaining heavy with light attacks
	yDir = y;
	OnResetDodgeBufferDelegate.Broadcast();

	if (iFighterRef->IsCurrentStateEqualToAny(attackCancelableStates))
	{
		//Save the input to buffer the next attack
		bSavedLightAttack = true;
		return;
	}
	if (CanAttack())
	{
		if (CanAerialAttack()) {PerformAttack(3);} //If Can perform an aerial attack, do it
		else if (yDir < 0) {PerformLaunchAttack();} //Else if player is holding back on left stick, perform launch attack
		else if (bHeavyAttack) {PerformComboStarter();} //Else if a heavy attack was performed previously, this will be the start of a combo
		else {PerformAttack(1);} //Else perform a light attack
	}
}

void UCombatComponent::HeavyAttack()
{
	if (heavyMeleeMontages.IsEmpty()) {return;}
	if (iFighterRef == nullptr || iPlayerRef == nullptr) {return;}
	if (!bRageMode) //If player isn't in rage mode, they need to have enough stamina
	{
		if (!iPlayerRef->HasEnoughStamina(heavyMeleeStaminaCost)) {return;}
	}

	bSavedLightAttack = false; //Bookeeping since we're performing a heavy attack; necessary for chaining light with heavy attacks
	OnResetDodgeBufferDelegate.Broadcast();
	bHeavyAttack = true;

	if (iFighterRef->IsCurrentStateEqualToAny(attackCancelableStates))
	{
		//Save the input to buffer the next attack
		bSavedHeavyAttack = true;
		return;
	}
	if (CanAttack())
	{
		if (CanSmashAttack()) {PerformSmashAttack();}
		else if (!bComboStarter) {PerformAttack(2);}
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
	//Ensures that if the player is in the air, we only reset their state when they hit the ground
	if (movementComp->MovementMode == MOVE_Flying)
	{
		movementComp->SetMovementMode(MOVE_Falling);
		bCanResetAttack = true;
	}
	else if (movementComp->MovementMode != MOVE_Falling)
	{
		iFighterRef->SetState(EState::NoneState);
		bHeavyAttack = false;
		bSavedLightAttack = false;
		bSavedHeavyAttack = false;
		bSavedRageMode = false;
		bCanAerialAttack = true;
		bCanSmashAttack = true;
		if (bRageMode) {bCanLoseRage = true;}
		yDir = 0;
	}
}

void UCombatComponent::ResetCombo()
{
	bComboStarter = false;
	comboStarterIndex = 0;
	comboCounter = 0;
}

void UCombatComponent::SaveLightAttack()
{
	if (!bSavedLightAttack) {return;}
	bSavedLightAttack = false;
	
	if (bRageMode) {bCanLoseRage = true;}

	if (iFighterRef->IsCurrentStateEqualToAny(attackCancelableStates)) {iFighterRef->SetState(EState::NoneState);}
	LightAttack(yDir);
}

void UCombatComponent::SaveHeavyAttack()
{
	if (!bSavedHeavyAttack) {return;}
	bSavedHeavyAttack = false;
	
	if (bRageMode) {bCanLoseRage = true;}

	if (iFighterRef->IsCurrentStateEqualToAny(attackCancelableStates)) {iFighterRef->SetState(EState::NoneState);}
	HeavyAttack();
}

void UCombatComponent::SaveRageMode()
{
	if (!bSavedRageMode) {return;}
	bSavedRageMode = false;

	if (iFighterRef->IsCurrentStateEqualToAny(attackCancelableStates)) {iFighterRef->SetState(EState::NoneState);}
	EnterRageMode();
}

void UCombatComponent::TryResetAttack()
{
	iFighterRef->ResumeKnockedDBMontage();
	if (!bCanResetAttack) {return;}
	bCanResetAttack = false;
	movementComp->SetMovementMode(MOVE_Walking);
	HandleResetAttack();
}
/************************************Public Functions************************************/