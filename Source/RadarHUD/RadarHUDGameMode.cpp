// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RadarHUDGameMode.h"
#include "RadarHUDHUD.h"
#include "RadarHUDCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARadarHUDGameMode::ARadarHUDGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARadarHUDHUD::StaticClass();
}
