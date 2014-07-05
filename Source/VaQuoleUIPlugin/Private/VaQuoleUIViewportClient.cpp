// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

UVaQuoleUIViewportClient::UVaQuoleUIViewportClient(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UVaQuoleUIViewportClient::RegisterWebUI(UVaQuoleUIComponent* WebView)
{
	UE_LOG(LogVaQuole, Warning, TEXT("Register UI"));
	HudViews.AddUnique(WebView);
}

void UVaQuoleUIViewportClient::UnregisterWebUI(UVaQuoleUIComponent* WebView)
{
	HudViews.Remove(WebView);
}

bool UVaQuoleUIViewportClient::InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	if (InViewport->IsPlayInEditorViewport() && Key.IsGamepadKey())
	{
		GEngine->RemapGamepadControllerIdForPIE(this, ControllerId);
	}

	// Route to subsystems that care
	bool bResult = (ViewportConsole ? ViewportConsole->InputKey(ControllerId, Key, EventType, AmountDepressed, bGamepad) : false);
	if (!bResult)
	{
		ULocalPlayer* const TargetPlayer = GEngine->GetLocalPlayerFromControllerId(this, ControllerId);
		if (TargetPlayer && TargetPlayer->PlayerController)
		{
			UE_LOG(LogVaQuole, Warning, TEXT("Key: %s"), *Key.ToString());

			// Process input with HUD views
			for (auto WebView : HudViews)
			{
				bResult = WebView->InputKey(Viewport, ControllerId, Key, EventType, AmountDepressed, bGamepad);

				// If view consumed input, break the loop
				if (bResult)
				{
					break;
				}
			}

			// @TODO Give a chance to UIs we're looking at

			// Give a chance to player controller
			if (!bResult)
			{
				bResult = TargetPlayer->PlayerController->InputKey(Key, EventType, AmountDepressed, bGamepad);
			}
		}

		// A gameviewport is always considered to have responded to a mouse buttons to avoid throttling
		if (!bResult && Key.IsMouseButton())
		{
			bResult = true;
		}
	}

	// For PIE, let the next PIE window handle the input if we didn't
	// (this allows people to use multiple controllers to control each window)
	if (!bResult && ControllerId > 0 && InViewport->IsPlayInEditorViewport())
	{
		UGameViewportClient *NextViewport = GEngine->GetNextPIEViewport(this);
		if (NextViewport)
		{
			bResult = NextViewport->InputKey(InViewport, ControllerId - 1, Key, EventType, AmountDepressed, bGamepad);
		}
	}

	return bResult;
}
