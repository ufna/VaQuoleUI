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

void AVaQuoleUIPlayerController::InputKeyQ(FViewport* Viewport, FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	FVector ActorImpact;
	UVaQuoleUIComponent* View;
	UStaticMeshComponent* Mesh;

	// Check we're targeting UI on scene
	if (GetTargetUIComponent(ActorImpact, &View, &Mesh))
	{
		View->InputKeyQ(Viewport, Key, EventType, AmountDepressed, bGamepad);
	}

	// Check we have HUD
	AVaQuoleHUD* MyHUD = GetVaQuoleHUD();
	if (MyHUD && MyHUD->VaQuoleUI.IsValid())
	{
		MyHUD->VaQuoleUI->InputKeyQ(Viewport, Key, EventType, AmountDepressed, bGamepad);
	}
}

void AVaQuoleUIPlayerController::MouseEvent(EMouseEvent::Type MouseEvent, VaQuole::EMouseButton::Type Button)
{
	FVector ActorImpact;
	UVaQuoleUIComponent* View;
	UStaticMeshComponent* Mesh;

	if (GetTargetUIComponent(ActorImpact, &View, &Mesh))
	{
		FVector Min, Max;

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

void AVaQuoleUIPlayerController::EvalJS(FString ScriptSource)
{
	// Check we have HUD
	AVaQuoleHUD* MyHUD = GetVaQuoleHUD();
	if (MyHUD && MyHUD->VaQuoleUI.IsValid() && MyHUD->VaQuoleUI->GetUIWidget().IsValid())
	{
		MyHUD->VaQuoleUI->GetUIWidget()->EvaluateJavaScript(*ScriptSource);
	}
}


//////////////////////////////////////////////////////////////////////////
// Internal helpers

AVaQuoleHUD* AVaQuoleUIPlayerController::GetVaQuoleHUD() const
{
	return Cast<AVaQuoleHUD>(GetHUD());
}

bool AVaQuoleUIPlayerController::GetTargetUIComponent(FVector& ActorImpact, UVaQuoleUIComponent** View, UStaticMeshComponent** Mesh)
{
	FVector TraceStart;
	FVector TraceDirection;
	FVector TraceEnd;

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer == NULL || LocalPlayer->ViewportClient == NULL || LocalPlayer->ViewportClient->Viewport == NULL)
	{
		return false;
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
		UVaQuoleUIComponent* TargetView = TraceHit.Actor->FindComponentByClass<UVaQuoleUIComponent>();

		if (TargetView != NULL)
		{
			// We suggest that UI applied to static mesh
			UStaticMeshComponent* TargetMesh = TraceHit.Actor->FindComponentByClass<UStaticMeshComponent>();

			if (TargetMesh != NULL)
			{
				FTransform ToActor = TraceHit.Actor->GetTransform().InverseSafe();
				FVector TargetActorImpact = ToActor.TransformPosition(TraceHit.ImpactPoint);

				ActorImpact = TargetActorImpact;
				*View = TargetView;
				*Mesh = TargetMesh;

				return true;
			}
		}
	}

	return false;
}

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
