// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnOffComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
//#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

	if (!bLockedOn || !IsValid(playerController) || !IsValid(currentTargetActor)) {return;}
	SetPlayerControlRotation();
}

void ULockOnOffComponent::GetReferences()
{
	ownerRef = GetOwner<ACharacter>();
	if (ownerRef == nullptr) {return;}
	playerController = ownerRef->GetController<APlayerController>();
	characterMovementComponent = ownerRef->GetCharacterMovement();
	springArmComponent = ownerRef->FindComponentByClass<USpringArmComponent>();
	camComp = ownerRef->FindComponentByClass<UCameraComponent>();
}

/************************************Private Functions************************************/
void ULockOnOffComponent::FindClosestEnemy(FVector startLocation)
{
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

		//distanceToTarget = a value between 0 and 1
		//0 = not close | 1 = close
		double angleToTarget = GetAngleToTarget(startLocation, endLocation);
		UE_LOG(LogTemp, Warning, TEXT("Angle to target: %f"), angleToTarget);
		if (angleToTarget > dotProduct) //If this enemy is closer than the previous one
		{
			//This enemy is now the current target actor
			dotProduct = angleToTarget;
			currentTargetActor = enemy;
		}
	}
}

void ULockOnOffComponent::FindClosestLeftEnemy(FVector startLocation)
{
	AActor* temp = currentTargetActor; //Used to make sure the player can't switch to the current target
	dotProduct = 0;
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

		GetWorld()->LineTraceSingleByChannel(outHit, camLocation, endLocation, ECC_GameTraceChannel1, ignoreParams);
		FVector rightVec {camComp->GetRightVector()};
		FVector normalVec {outHit.Normal};
		double angle {FVector::DotProduct(rightVec, normalVec)};
		//If enemy isn't on the left side of the player or is the same as current target, skip it
		if (angle <= 0 || temp == enemy) {continue;}

		//distanceToTarget = a value between 0 and 1
		//0 = not close | 1 = close
		double angleToTarget = GetAngleToTarget(startLocation, endLocation);
		UE_LOG(LogTemp, Warning, TEXT("Angle to target: %f | %s"), angleToTarget, *outHit.GetActor()->GetName());
		if (angleToTarget > dotProduct) //If this enemy is closer than the previous one
		{
			IEnemy::Execute_OnDeselect(currentTargetActor);
			//This enemy is now the current target actor
			dotProduct = angleToTarget;
			currentTargetActor = enemy;
		}
	}
}

void ULockOnOffComponent::FindClosestRightEnemy(FVector startLocation)
{
	AActor* temp = currentTargetActor; //Used to make sure the player can't switch to the current target
	dotProduct = 0;
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

		GetWorld()->LineTraceSingleByChannel(outHit, camLocation, endLocation, ECC_GameTraceChannel1, ignoreParams);
		FVector rightVec {camComp->GetRightVector()};
		FVector normalVec {outHit.Normal};
		double angle {FVector::DotProduct(rightVec, normalVec)};
		//If enemy isn't on the right side of the player or is the same as current target, skip it
		if (angle >= 0 || temp == enemy) {continue;}

		//distanceToTarget = a value between 0 and 1
		//0 = not close | 1 = close
		double angleToTarget = GetAngleToTarget(startLocation, endLocation);
		UE_LOG(LogTemp, Warning, TEXT("Angle to target: %f | %s"), angleToTarget, *outHit.GetActor()->GetName());
		if (angleToTarget > dotProduct) //If this enemy is closer than the previous one
		{
			IEnemy::Execute_OnDeselect(currentTargetActor);
			//This enemy is now the current target actor
			dotProduct = angleToTarget;
			currentTargetActor = enemy;
		}
	}
}

//Whicever enemy yields the smallest dot product will be who we initially lock onto
//when swithing between locked on enemies, we'll switch by the smallest dot products
double ULockOnOffComponent::GetAngleToTarget(FVector playerLoc, FVector enemyLoc) const
{
	FRotator desiredRot {UKismetMathLibrary::FindLookAtRotation(playerLoc, enemyLoc)}; //Rotation the player needs to face the enemy
	FVector xVec {UKismetMathLibrary::Conv_RotatorToVector(desiredRot)}; //X is forward
	FVector camFwdVec {camComp->GetForwardVector()}; //Forward direction the player camera is facing
	return FVector::DotProduct(camFwdVec, xVec); //How close the player and their camera are to facing the same direction
}

void ULockOnOffComponent::LockOn()
{
	if (currentTargetActor == nullptr) //Should never be true, but just a safety precaution
	{
		UE_LOG(LogTemp, Warning, TEXT("Current target actor is null"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Locking on to: %s"), *currentTargetActor->GetName());

	characterMovementComponent->bUseControllerDesiredRotation = true;
	springArmComponent->TargetOffset = cameraOffset;

	bLockedOn = true;
	IEnemy::Execute_OnSelect(currentTargetActor);
}

void ULockOnOffComponent::SetPlayerControlRotation()
{
	FVector currentLocation {ownerRef->GetActorLocation()};
	FVector targetLocation {currentTargetActor->GetActorLocation()};

	//Can't lock on to target if they're out of range
	double distance {FVector::Distance(currentLocation, targetLocation)};
	if (distance > traceRadius)
	{
		LockOff();
		return;
	}

	targetLocation.Z += lockedOnCameraTiltZ; //Tilt the camera to a desirable rotation
	//Rotates the player towards their target
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
	newRotation = UKismetMathLibrary::RInterpTo_Constant(playerController->GetControlRotation(), newRotation, GetWorld()->GetDeltaSeconds(), cameraInterpSpeed);
	playerController->SetControlRotation(newRotation);
}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void ULockOnOffComponent::ToggleLockOnOff()
{
	if (bLockedOn) {LockOff();}
	else {FindActorsToLockOnTo(0.0f);}
}
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
void ULockOnOffComponent::FindActorsToLockOnTo(float yaw)
{
	/****************SPHERE TRACE AROUND THE PLAYER AND CATALOG ANY ENEMIES HIT****************************/
	TArray<FHitResult> outHits;
	FVector startLocation = ownerRef->GetActorLocation();
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
	if (yaw < 0) {FindClosestLeftEnemy(startLocation);}
	else if (yaw > 0) {FindClosestRightEnemy(startLocation);}
	else {FindClosestEnemy(startLocation);}
	LockOn();
}

bool ULockOnOffComponent::GetLockedOn() {return bLockedOn;}

void ULockOnOffComponent::LockOff()
{
	bLockedOn = false;
	dotProduct = 0.0f;
	springArmComponent->TargetOffset = FVector::ZeroVector;
	characterMovementComponent->bUseControllerDesiredRotation = false;
	if (IsValid(currentTargetActor)) {IEnemy::Execute_OnDeselect(currentTargetActor);}
	enemies.Empty();
	currentTargetActor = nullptr;
}
/************************************Public Functions************************************/