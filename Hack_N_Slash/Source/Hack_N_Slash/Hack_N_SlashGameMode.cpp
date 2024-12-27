// Copyright Epic Games, Inc. All Rights Reserved.

#include "Hack_N_SlashGameMode.h"
#include "Hack_N_SlashCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHack_N_SlashGameMode::AHack_N_SlashGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
