// Copyright Epic Games, Inc. All Rights Reserved.

#include "Evil_Bun_DungeonGameMode.h"
#include "Evil_Bun_DungeonPlayerController.h"
#include "Evil_Bun_DungeonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEvil_Bun_DungeonGameMode::AEvil_Bun_DungeonGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AEvil_Bun_DungeonPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}