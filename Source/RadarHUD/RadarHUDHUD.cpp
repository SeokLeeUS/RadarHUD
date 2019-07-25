// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RadarHUDHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

ARadarHUDHUD::ARadarHUDHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void ARadarHUDHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );


	DrawRadar();

	DrawPlayerInRadar();

	PerformRadarRaycast();


	DrawRaycastedActors();

	//Empty the radar actors in case the player moves out of range,
	//by doing so, we have always a valid display in our radar
	RadarActors.Empty();
}

FVector2D ARadarHUDHUD::ConvertWorldLocationToLocal(AActor * ActorToPlace)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player && ActorToPlace)
	{

		FVector ActorsLocal3dVector = Player->GetTransform().InverseTransformPosition(ActorToPlace->GetActorLocation());
		ActorsLocal3dVector = FRotator(0.f, -90.f, 0.f).RotateVector(ActorsLocal3dVector);

		ActorsLocal3dVector /= RadarDistanceScale;

		return FVector2D(ActorsLocal3dVector);
	}
	return FVector2D(0, 0);
}

FVector2D ARadarHUDHUD::GetRadarCenterPosition()
{
	if (Canvas)
	{
		return FVector2D(Canvas->SizeX*RadarStartLocation.X, Canvas->SizeY*RadarStartLocation.Y);
	}
	else
	{
		return FVector2D(0, 0);
	}
}

void ARadarHUDHUD::DrawRadar()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	for (float i = 0; i < 360; i += DegreeStep)
	{
		float fixedX = FMath::Cos(i) * RadarRadius;
		float fixedY = FMath::Sin(i) * RadarRadius;

		DrawLine(RadarCenter.X, RadarCenter.Y, RadarCenter.X + fixedX, RadarCenter.Y + fixedY, FLinearColor::Gray, 1.f);
	}

}


void ARadarHUDHUD::DrawPlayerInRadar()
{
	FVector2D RadarCenter = GetRadarCenterPosition();
	DrawRect(FLinearColor::Blue, RadarCenter.X, RadarCenter.Y, DrawPixelSize, DrawPixelSize);
}


void ARadarHUDHUD::PerformRadarRaycast()
{
	APawn*Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player)
	{
		TArray<FHitResult> HitResults;
		FVector EndLocation = Player->GetActorLocation();
		EndLocation.Z += SphereHeight;

		FCollisionShape CollisionShape;
		CollisionShape.ShapeType = ECollisionShape::Sphere;
		CollisionShape.SetSphere(SphereRadius);

		GetWorld()->SweepMultiByChannel(HitResults, Player->GetActorLocation(), EndLocation, FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, CollisionShape);


		for (auto It : HitResults)
		{
			AActor* CurrentActor = It.GetActor();
			if (CurrentActor && CurrentActor->ActorHasTag("Radar"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Radar Detected"))
				RadarActors.Add(CurrentActor);
			}

		}

	}



}



void ARadarHUDHUD::DrawRaycastedActors()

{
	FVector2D RadarCenter = GetRadarCenterPosition();
	for (auto It : RadarActors)
	{
		FVector2D convertedLocation = ConvertWorldLocationToLocal(It);

		//We want to clamp the location of our actors in order to make sure
		//that we display them inside our radar

		//To do so, I've created the following temporary vector in order to access
		//the GetClampedToMaxSize2d function. This functions returns a clamped vector (if needed)
		//to match our max length
		FVector tempVector = FVector(convertedLocation.X, convertedLocation.Y, 0.f);

		//Subtract the pixel size in order to make the radar display more accurate
		tempVector = tempVector.GetClampedToMaxSize2D(RadarRadius - DrawPixelSize);

		//Assign the converted X and Y values to the vector we want to display
		convertedLocation.X = tempVector.X;
		convertedLocation.Y = tempVector.Y;

		DrawRect(FLinearColor::Red, RadarCenter.X + convertedLocation.X, RadarCenter.Y + convertedLocation.Y, DrawPixelSize, DrawPixelSize);
	}
}
