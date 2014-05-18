// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleUIViewportClient.generated.h"

UCLASS()
class UVaQuoleUIViewportClient : public UGameViewportClient
{
	GENERATED_UCLASS_BODY()

	// FViewportClient interface.
	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed = 1.f, bool bGamepad = false) OVERRIDE;
	// End of FViewportClient interface.
};
