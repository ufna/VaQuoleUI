// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleUIViewportClient.generated.h"

UCLASS()
class VAQUOLEUIPLUGIN_API UVaQuoleUIViewportClient : public UGameViewportClient
{
	GENERATED_UCLASS_BODY()

	/** Register UI component as a HUD UI */
	void RegisterHudUI(UVaQuoleUIComponent* WebView);

	/** Unregister HUD UI component */
	void UnregisterHudUI(UVaQuoleUIComponent* WebView);

	/** Register UI component as a Scene UI */
	void RegisterSceneUI(UVaQuoleUIComponent* WebView);

	/** Unregister Scene UI component */
	void UnregisterSceneUI(UVaQuoleUIComponent* WebView);

	// FViewportClient interface.
	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed = 1.f, bool bGamepad = false) override;
	// End of FViewportClient interface.

private:
	/** List of all HUD web views (UI of this kind can be operated in 2D mode). High priority to grab input. */
	TArray<UVaQuoleUIComponent*> HudViews;

	/** List of all scene web views. Low priority to grab input. */
	TArray<UVaQuoleUIComponent*> SceneViews;

};
