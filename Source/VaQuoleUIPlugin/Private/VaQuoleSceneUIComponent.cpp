// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

UVaQuoleSceneUIComponent::UVaQuoleSceneUIComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bEnabled = false;
	bTransparent = false;

	Width = 256;
	Height = 256;

	DefaultURL = "http://html5test.com";

	TextureParameterName = TEXT("VaQuoleUITexture");

	SurfaceMapping = ESurfaceMapping::Planar;

	bRegisteredUI = false;
}

void UVaQuoleSceneUIComponent::BeginDestroy()
{
	// Force unregister page from Viewport
	RegisterInput(false);

	Super::BeginDestroy();
}


//////////////////////////////////////////////////////////////////////////
// View control

bool UVaQuoleSceneUIComponent::IsInputRegistered()
{
	return bRegisteredUI;
}

void UVaQuoleSceneUIComponent::RegisterInput(bool bRegisterInput)
{
	if (GEngine && GEngine->GameViewport)
	{
		UVaQuoleUIViewportClient* ViewportClient = Cast<UVaQuoleUIViewportClient>(GEngine->GameViewport);
		if (ViewportClient)
		{
			if (bRegisterInput)
			{
				ViewportClient->RegisterSceneUI(this);
			}
			else
			{
				ViewportClient->UnregisterSceneUI(this);
			}

			bRegisteredUI = bRegisterInput;
		}
	}
}

void UVaQuoleSceneUIComponent::UnregisterInput()
{
	RegisterInput(false);
}

void UVaQuoleSceneUIComponent::MouseMove(float X, float Y)
{
	SetMousePosition(X, Y);
}

bool UVaQuoleSceneUIComponent::MouseMoveFromHitResult(const FHitResult& HitResult)
{
	AActor* Owner = GetOwner();
	if (Owner == NULL)
	{
		return false;
	}

	// We suggest that UI applied to static mesh
	UStaticMeshComponent* TargetMesh = Owner->FindComponentByClass<UStaticMeshComponent>();
	if (TargetMesh != NULL)
	{
		FTransform ToActor = HitResult.Actor->GetTransform().Inverse();
		FVector ActorImpact = ToActor.TransformPosition(HitResult.ImpactPoint);

		FVector Min, Max;
		TargetMesh->GetLocalBounds(Min, Max);

		float SizeX = (Max.X - Min.X);
		float SizeY = (Max.Y - Min.Y);
		float SizeZ = (Max.Z - Min.Z);

		if (SurfaceMapping == ESurfaceMapping::Planar)
		{
			// Ensure we haven't zero subdivison
			if (SizeY > 0.f && SizeX > 0.f)
			{
				float X = GetWidth() * (Max.Y - ActorImpact.Y) / SizeY;
				float Y = GetHeight() * (Max.Z - ActorImpact.Z) / SizeZ;

				MouseMove(X, Y);
			}
		}
		else if (SurfaceMapping = ESurfaceMapping::Cylindrical)
		{
			// Ensure that we won't get /0 error
			if (SizeX > 0.f && SizeY > 0.f && SizeZ > 0.f)
			{
				float XNormal = 2.f * (ActorImpact.X - Min.X) / SizeX - 1.f;
				float YNormal = 2.f * (ActorImpact.Y - Min.Y) / SizeY - 1.f;

				float Radius = 0.f;
				float Angle = 0.f;

				FMath::CartesianToPolar(XNormal, -YNormal, Radius, Angle);

				float X = GetWidth() * (FMath::RadiansToDegrees(Angle) + 180.f) / 360.f;
				float Y = GetHeight() * (Max.Z - ActorImpact.Z) / SizeZ;

				MouseMove(X, Y);

				return true;
			}
		}

		return false;
	}

	return false;
}
