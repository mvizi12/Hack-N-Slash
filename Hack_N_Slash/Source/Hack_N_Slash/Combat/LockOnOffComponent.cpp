// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnOffComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
//#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
//#include "GameFramework/CharacterMovementComponent.h"
//#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Enemy.h"

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

//Whicever enemy yields the smallest dot product will be who we initially lock onto
//when swithing between locked on enemies, we'll switch by the smallest dot products
float ULockOnOffComponent::GetDistanceToTarget(FVector playerLoc, FVector enemyLoc)
{
	FRotator desiredRot {UKismetMathLibrary::FindLookAtRotation(playerLoc, enemyLoc)}; //Rotation the player needs to face the enemy
	FVector xVec {UKismetMathLibrary::Conv_RotatorToVector(desiredRot)}; //X is forward
	FVector camFwdVec {camComp->GetForwardVector()}; //Forward direction the player camera is facing
	return FVector::DotProduct(camFwdVec, xVec); //How close the player and their camera are to facing the same direction
}

void ULockOnOffComponent::FindActorsToLockOnTo(float traceRadius, FVector cameraOffset)
{
	/****************SPHERE TRACE AROUND THE PLAYER AND CATALOG ANY ENEMIES HIT****************************/
	TArray<FHitResult> outHits;
	FVector startLocation = ownerRef->GetActorLocation();
	//FVector endLocation = traceDistance * ownerRef->GetActorForwardVector() + startLocation;
	TArray<AActor*> temp {ownerRef}; //Ignore self

	if (bDebugMode) {bool targetFound = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), startLocation, startLocation, traceRadius, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, temp, EDrawDebugTrace::ForDuration, outHits, true, FLinearColor::Red, FLinearColor::Green, 1.0f);}
	else {bool targetFound = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), startLocation, startLocation, traceRadius, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, temp, EDrawDebugTrace::None, outHits, true, FLinearColor::Red, FLinearColor::Green);}

	if (outHits.Num() <= 0) {return;}

	for (const FHitResult& hit : outHits)
	{
		AActor* enemy = hit.GetActor();
		if (!enemy->Implements<UEnemy>()) {continue;} //If hit actor doesn't implement "Enemy" interface, continue to next loop
		enemies.AddUnique(enemy);
	}
	/****************SPHERE TRACE AROUND THE PLAYER AND CATALOG ANY ENEMIES HIT****************************/

	/*********************************DETERMINE WHICH ACTOR TO LOCK ON TO**********************************/
	for (AActor* enemy : enemies)
	{
		FHitResult outHit;
		FVector camLocation {camComp->GetComponentLocation()};
		FVector endLocation = enemy->GetActorLocation();
		FCollisionQueryParams ignoreParams {FName {TEXT("Ignore Collision Parameters")}, false, ownerRef};
		bool bHit = GetWorld()->LineTraceSingleByChannel(outHit, camLocation, endLocation, ECC_Visibility, ignoreParams);
		if (bDebugMode) {DrawDebugLine(GetWorld(), camLocation, endLocation, bHit ? FColor::Yellow : FColor::Blue, false, 1.0f, 0, 1.0f);}

		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Object blocking sight: %s"), *outHit.GetActor()->GetName());
			continue;
		}

		float distanceToPlayer = GetDistanceToTarget(startLocation, endLocation);
		UE_LOG(LogTemp, Warning, TEXT("Distance to player: %f"), distanceToPlayer);
		if (distanceToPlayer < dotProduct) //If this enemy is closer than the previous one
		{
			//This enemy is now the current target actor
			dotProduct = distanceToPlayer;
			currentTargetActor = enemy;
		}
	}
	if (currentTargetActor == nullptr) //should never be true, but just a safety precaution
	{
		UE_LOG(LogTemp, Warning, TEXT("Current target actor is null"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Locking on to: %s"), *currentTargetActor->GetName());
	/*********************************DETERMINE WHICH ACTOR TO LOCK ON TO**********************************/

	bLockedOn = true;
	IEnemy::Execute_OnSelect(currentTargetActor);
}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void ULockOnOffComponent::ToggleLockOnOff(float traceRadius, FVector cameraOffset)
{
	if (bLockedOn) {LockOff();}
	else {FindActorsToLockOnTo(traceRadius, cameraOffset);}
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