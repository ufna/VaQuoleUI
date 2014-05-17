// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

UVaQuoleUIViewportClient::UVaQuoleUIViewportClient(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

bool UVaQuoleUIViewportClient::InputKey(FViewport* Viewport, int32 ControllerId,
	FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	ULocalPlayer* const TargetPlayer = GEngine->GetLocalPlayerFromControllerId(this, ControllerId);
	if (TargetPlayer && TargetPlayer->PlayerController)
	{
		AVaQuoleUIPlayerController* MyPlayer = Cast<AVaQuoleUIPlayerController>(TargetPlayer->PlayerController);
		if (MyPlayer)
		{
			MyPlayer->InputKeyQ(Viewport, Key, EventType, AmountDepressed, bGamepad);
		}
	}

	return Super::InputKey(Viewport, ControllerId, Key, EventType, AmountDepressed, bGamepad);
}
