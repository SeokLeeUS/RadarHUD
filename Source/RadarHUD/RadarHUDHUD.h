// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RadarHUDHUD.generated.h"

UCLASS()
class ARadarHUDHUD : public AHUD
{
	GENERATED_BODY()

public:
	ARadarHUDHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	/*the pixel size of the drawable radar actors*/



private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;


protected:

	/*The start location of our radar*/
	UPROPERTY(EditAnywhere, Category = Radar)
	FVector2D RadarStartLocation = FVector2D(0.9f, 0.2f);

	/*The radius of our radar*/
	UPROPERTY(EditAnywhere, Category = Radar)
	float RadarRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = Radar)
	float DegreeStep = 0.25f;

	UPROPERTY(EditAnywhere, Category = Radar)
	float SphereHeight = 200.f;

	UPROPERTY(EditAnywhere, Category = Radar)
	float SphereRadius = 2750.f;

	UPROPERTY(EditAnywhere, Category = Radar)
	float DrawPixelSize = 5.f;

	UPROPERTY(EditAnywhere, Category = Radar)
	float RadarDistanceScale = 25.f;

	FVector2D ConvertWorldLocationToLocal(AActor* ActorToPlace);
	
	
	TArray<AActor*> RadarActors;


	/*Returns the center of the radar as a 2d vector*/
	FVector2D GetRadarCenterPosition();

	/*Draws the radar*/
	void DrawRadar();

	void DrawPlayerInRadar();

	void PerformRadarRaycast();

	void DrawRaycastedActors();
};

