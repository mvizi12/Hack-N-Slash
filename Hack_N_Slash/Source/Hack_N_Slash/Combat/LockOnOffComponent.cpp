// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnOffComponent.h"
#include "Kismet/KismetSystemLibrary.h"
//#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Enemy.h"
#include "Kismet/KismetMathLibrary.h"

ULockOnOffComponent::ULockOnOffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void ULockOnOffComponent::BeginPlay()
{
	Super::BeginPlay();

	GetReferences();
}

void ULockOnOffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


/************************************Private Functions************************************/
void ULockOnOffComponent::GetReferences()
{
	ownerRef = GetOwner<ACharacter>();
	if (ownerRef == nullptr) {return;}
	//playerController = ownerRef->GetController<APlayerController>();
	//characterMovementComponent = ownerRef->GetCharacterMovement();
	//springArmComponent = ownerRef->FindComponentByClass<USpringArmComponent>();
	camComp = ownerRef->FindComponentByClass<UCameraComponent>();
}

void ULockOnOffComponent::LockOn(float traceRadius, FVector cameraOffset)
{
	TArray<FHitResult> outHits;
	FVector startLocation = ownerRef->GetActorLocation();
	//FVector endLocation = traceDistance * ownerRef->GetActorForwardVector() + startLocation;
	TArray<AActor*> temp {ownerRef}; //Ignore self

	if (bDebugMode) {bool targetFound = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), startLocation, startLocation, traceRadius, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), false, temp, EDrawDebugTrace::ForDuration, outHits, true, FLinearColor::Red, FLinearColor::Green, 1.0f);}
	else {bool targetFound = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), startLocation, startLocation, traceRadius, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), false, temp, EDrawDebugTrace::None, outHits, true, FLinearColor::Red, FLinearColor::Green);}

	if (outHits.Num() <= 0) {return;}

	for (const FHitResult& hit : outHits)
	{
		if (!hit.GetActor()->Implements<UEnemy>()) {continue;} //If hit actor doesn't implement "Enemy" interface, continue to next loop
		enemyHits.AddUnique(hit);
	}

	//Determine which actor to lock on to
	for (const FHitResult& hit : enemyHits)
	{
		FHitResult outHit;
		FVector endLocation = hit.GetActor()->GetActorLocation();
		//FCollisionShape sphere {FCollisionShape::MakeSphere(50.0f)};
		FCollisionQueryParams ignoreParams {FName {TEXT("Ignore Collision Parameters")}, false, ownerRef};
		//if (!GetWorld()->SweepSingleByChannel(outHit, startLocation, endLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, sphere, ignoreParams)) {return;}
		if (GetWorld()->LineTraceSingleByChannel(outHit, startLocation, endLocation, ECollisionChannel::ECC_Visibility, ignoreParams)) {continue;} //Something is blocking sight to the enemy
	}

	bLockedOn = true;
	IEnemy::Execute_OnSelect(currentTargetActor);
}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void ULockOnOffComponent::ToggleLockOnOff(float traceRadius, FVector cameraOffset)
{
	if (bLockedOn) {LockOff();}
	else {LockOn(traceRadius, cameraOffset);}
}
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
bool ULockOnOffComponent::GetLockedOn()
{
    return bLockedOn;
}

void ULockOnOffComponent::LockOff()
{
	bLockedOn = false;
	IEnemy::Execute_OnDeselect(currentTargetActor);
}
/************************************Public Functions************************************/