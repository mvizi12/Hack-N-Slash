// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
//#include "Engine/DamageEvents.h"
#include "C:\Users\mvizi\Documents\Unreal Projects\Hack-N-Slash\Hack_N_Slash\Source\Hack_N_Slash\Interfaces\Fighter.h"
#include "Engine/EngineTypes.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	GetReferences();
}

// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/************************************Private Functions************************************/
void UTraceComponent::GetReferences()
{
	ownerRef = GetOwner();
}

void UTraceComponent::HandleDamage(TArray<FHitResult> &outHits, float damage)
{
	IFighter* iFighterRef {Cast<IFighter>(ownerRef)};
	if (iFighterRef == nullptr) {return;}
	damage += iFighterRef->GetStrength();
	
	for (const FHitResult& hitResult : outHits)
	{
		AActor* targetActor {hitResult.GetActor()};
		if (targetActor == nullptr) {continue;}
		//UGameplayStatics::ApplyDamage(targetActor, damage, ownerRef->GetInstigatorController(), ownerRef);
		//FDamageEvent Event(damageType);
		//targetActor->TakeDamage(10.0f, Event, ownerRef->GetInstigatorController(), ownerRef);
		OnReportDamageDelegate.Broadcast(targetActor, damage, targetActor->GetActorLocation());
	}
}
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
void UTraceComponent::WeaponTrace(float traceDistance, float traceRadius, float damage)
{
	TArray<FHitResult> outHits;
	FVector startLocation = ownerRef->GetActorLocation();
	FVector endLocation = traceDistance * ownerRef->GetActorForwardVector() + startLocation;
	TArray<AActor*> actorsToIgnore; //Empty array

	if (bDebugMode) {bool targetFound = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), startLocation, endLocation, traceRadius, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), false, actorsToIgnore, EDrawDebugTrace::ForDuration, outHits, true, FLinearColor::Red, FLinearColor::Green, 1.0f);}
	else {bool targetFound = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), startLocation, endLocation, traceRadius, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), false, actorsToIgnore, EDrawDebugTrace::None, outHits, true, FLinearColor::Red, FLinearColor::Green);}

	if (outHits.Num() <= 0) {return;}
	HandleDamage(outHits, damage);
}
/************************************Public Functions************************************/