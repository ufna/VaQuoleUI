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
}

void UVaQuoleSceneUIComponent::BeginDestroy()
{
	// Force unregister page from Viewport
	if (GEngine && GEngine->GameViewport)
	{
		UVaQuoleUIViewportClient* ViewportClient = Cast<UVaQuoleUIViewportClient>(GEngine->GameViewport);
		if (ViewportClient)
		{
			ViewportClient->UnregisterSceneUI(this);
		}
	}

	Super::BeginDestroy();
}


//////////////////////////////////////////////////////////////////////////
// View control

void UVaQuoleSceneUIComponent::MouseMove(float X, float Y)
{
	SetMousePosition(X, Y);
}
