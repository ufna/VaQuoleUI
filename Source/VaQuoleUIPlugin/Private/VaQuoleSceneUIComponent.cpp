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
		FTransform ToActor = HitResult.Actor->GetTransform().InverseSafe();
		FVector ActorImpact = ToActor.TransformPosition(HitResult.ImpactPoint);

		FVector Min, Max;
		TargetMesh->GetLocalBounds(Min, Max);

		UE_LOG(LogVaQuole, Warning, TEXT("Min %s"), *Min.ToString());
		UE_LOG(LogVaQuole, Warning, TEXT("Max %s"), *Max.ToString());

		// @TODO Add rotation modificator
		int32 X = GetWidth() * (ActorImpact.X - Min.X) / (Max.X - Min.X);
		int32 Y = GetHeight() * (Max.Z - ActorImpact.Z) / (Max.Z - Min.Z);

		MouseMove(X, Y);

		UE_LOG(LogVaQuole, Warning, TEXT("Mouse X %d"), X);
		UE_LOG(LogVaQuole, Warning, TEXT("Mouse Y %d"), Y);

		return true;
	}

	return false;
}
