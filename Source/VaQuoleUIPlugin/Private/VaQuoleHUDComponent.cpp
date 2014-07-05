// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

UVaQuoleHUDComponent::UVaQuoleHUDComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bEnabled = true;
	bTransparent = true;

	Width = 256;
	Height = 256;

	DefaultURL = "http://html5test.com";

	TextureParameterName = TEXT("VaQuoleUITexture");
}

void UVaQuoleHUDComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetScreenPosition(0.f, 0.f);
	SetScreenSize(Width, Height);

	// Register page with Viewport Client to receive input
	if (GEngine && GEngine->GameViewport)
	{
		UVaQuoleUIViewportClient* ViewportClient = Cast<UVaQuoleUIViewportClient>(GEngine->GameViewport);
		if (ViewportClient)
		{
			ViewportClient->RegisterWebUI(this);
		}
	}
}

void UVaQuoleHUDComponent::BeginDestroy()
{
	// Unregister page from Viewport
	if (GEngine && GEngine->GameViewport)
	{
		UVaQuoleUIViewportClient* ViewportClient = Cast<UVaQuoleUIViewportClient>(GEngine->GameViewport);
		if (ViewportClient)
		{
			ViewportClient->UnregisterWebUI(this);
		}
	}

	Super::BeginDestroy();
}

void UVaQuoleHUDComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Update mouse position
	FVector2D MousePosition = FVector2D::ZeroVector;
	if (GetMouseScreenPosition(MousePosition))
	{
		float ScreenX = (MousePosition.X - ScreenPosition.X) * (Width / ScreenSize.X);
		float ScreenY = (MousePosition.Y - ScreenPosition.Y) * (Height / ScreenSize.Y);

		SetMousePosition(ScreenX, ScreenY);
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVaQuoleHUDComponent::Resize(int32 NewWidth, int32 NewHeight)
{
	Super::Resize(NewWidth, NewHeight);

	SetScreenSize(Width, Height);
}


//////////////////////////////////////////////////////////////////////////
// View control

FVector2D UVaQuoleHUDComponent::GetScreenPosition() const
{
	return ScreenPosition;
}

void UVaQuoleHUDComponent::SetScreenPosition(float ScreenX, float ScreenY)
{
	ScreenPosition = FVector2D(ScreenX, ScreenY);
}

FVector2D UVaQuoleHUDComponent::GetScreenSize() const
{
	return ScreenSize;
}

void UVaQuoleHUDComponent::SetScreenSize(float ScreenWidth, float ScreenHeight)
{
	ScreenSize = FVector2D(ScreenWidth, ScreenHeight);
}
