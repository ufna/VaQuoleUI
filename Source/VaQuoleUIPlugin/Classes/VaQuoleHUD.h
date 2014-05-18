// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleHUD.generated.h"

/**
 * Simple hud that processes things like hit notifications, etc.
 */
UCLASS()
class AVaQuoleHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	/** Web-based HUD rendering component */
	UPROPERTY(Category = VaQuole, VisibleAnywhere, BlueprintReadOnly)
	TSubobjectPtr<class UVaQuoleUIComponent> VaQuoleUI;

public:

	/** Main HUD update loop */
	virtual void DrawHUD() OVERRIDE;
};
