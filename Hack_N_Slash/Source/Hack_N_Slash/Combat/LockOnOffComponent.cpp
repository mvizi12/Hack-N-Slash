// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnOffComponent.h"

ULockOnOffComponent::ULockOnOffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void ULockOnOffComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULockOnOffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


/************************************Private Functions************************************/
/************************************Private Functions************************************/

/************************************Protected Functions************************************/
void ULockOnOffComponent::ToggleLockOnOff(float radius, FVector cameraOffset)
{
}
/************************************Protected Functions************************************/

/************************************Public Functions************************************/
bool ULockOnOffComponent::GetLockedOn()
{
    return bLockedOn;
}
/************************************Public Functions************************************/