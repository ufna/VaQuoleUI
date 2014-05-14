// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

AVaQuoleUIPlayerController::AVaQuoleUIPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bDebugMouseInput = false;
}

void AVaQuoleUIPlayerController::InitInputSystem()
{
	Super::InitInputSystem();

	BIND_ACTION(InputComponent, "MouseDown", IE_Pressed, &AVaQuoleUIPlayerController::MouseDown);
	BIND_ACTION(InputComponent, "MouseUp", IE_Released, &AVaQuoleUIPlayerController::MouseUp);
}

void AVaQuoleUIPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	MouseEvent(EMouseEvent::MouseMove, VaQuole::EMouseButton::NoButton);
}

void AVaQuoleUIPlayerController::MouseEvent(EMouseEvent::Type MouseEvent, VaQuole::EMouseButton::Type Button)
{
	FVector TraceStart;
	FVector TraceDirection;
	FVector TraceEnd;

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer == NULL || LocalPlayer->ViewportClient == NULL || LocalPlayer->ViewportClient->Viewport == NULL)
	{
		return;
	}

	// Project screen center as mouse cursor
	FSceneViewProjectionData ProjectionData;
	if (LocalPlayer->GetProjectionData(LocalPlayer->ViewportClient->Viewport, eSSP_FULL, /*out*/ ProjectionData))
	{
		const FMatrix InvViewMatrix = ProjectionData.ViewMatrix.Inverse();
		const FMatrix InvProjectionMatrix = ProjectionData.ProjectionMatrix.Inverse();
		FVector2D ScreenPosition(ProjectionData.GetViewRect().Width() / 2, ProjectionData.GetViewRect().Height() / 2);
		FSceneView::DeprojectScreenToWorld(ScreenPosition, ProjectionData.GetConstrainedViewRect(), InvViewMatrix, InvProjectionMatrix, /*out*/ TraceStart, /*out*/ TraceDirection);
	}

	TraceEnd = TraceStart + WORLD_MAX * TraceDirection;

	FHitResult TraceHit;
	if (GetWorld()->LineTraceSingle(TraceHit, TraceStart, TraceEnd, FCollisionQueryParams(true), FCollisionObjectQueryParams(ECC_WorldDynamic)))
	{
		UVaQuoleUIComponent* View = TraceHit.Actor->FindComponentByClass<UVaQuoleUIComponent>();
		if (View != NULL)
		{
			FTransform ToActor = TraceHit.Actor->GetTransform().InverseSafe();
			FVector ActorImpact = ToActor.TransformPosition(TraceHit.ImpactPoint);
			FVector Min, Max;

			FVector impact = TraceHit.ImpactPoint;
			FTransform transform = TraceHit.Actor->GetTransform();
			FTransform toObject = transform.InverseSafe();
			FVector objectImpact = toObject.TransformPosition(impact);
			ActorImpact = objectImpact;

			// We suggest that UI applied to static mesh
			UStaticMeshComponent* Mesh = TraceHit.Actor->FindComponentByClass<UStaticMeshComponent>();
			Mesh->GetLocalBounds(Min, Max);

			if (bDebugMouseInput)
			{
				UE_LOG(LogVaQuole, Warning, TEXT("Min %s"), *Min.ToString());
				UE_LOG(LogVaQuole, Warning, TEXT("Max %s"), *Max.ToString());
			}

			// @TODO Add rotation modificator
			int32 X = View->GetWidth() * (ActorImpact.X - Min.X) / (Max.X - Min.X);
			int32 Y = View->GetHeight() * (Max.Z - ActorImpact.Z) / (Max.Z - Min.Z);

			switch (MouseEvent)
			{
			case EMouseEvent::MouseMove:
				View->MouseMove(X, Y);
				break;

			case EMouseEvent::MousePressed:
				View->MouseClick(X, Y, Button, true);
				break;

			case EMouseEvent::MouseReleased:
				View->MouseClick(X, Y, Button, false);
				break;

			default:
				break;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Mouse input example

void AVaQuoleUIPlayerController::MouseDown()
{
	MouseEvent(EMouseEvent::MousePressed, VaQuole::EMouseButton::LeftButton);
}

void AVaQuoleUIPlayerController::MouseUp()
{
	MouseEvent(EMouseEvent::MouseReleased, VaQuole::EMouseButton::LeftButton);
}


//////////////////////////////////////////////////////////////////////////
// Internal helpers

bool AVaQuoleUIPlayerController::GetMouseScreenPosition(FVector2D& MousePosition)
{
#if PLATFORM_DESKTOP
	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		MousePosition = LocalPlayer->ViewportClient->GetMousePosition();
		return true;
	}
#endif

	return false;
}
