// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleUIPlayerController.generated.h"

/** Helper to control mouse */
UENUM(BlueprintType)
namespace EMouseEvent
{
	enum Type
	{
		MouseMove,
		MousePressed,
		MouseReleased
	};
}

UCLASS()
class AVaQuoleUIPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	/** Attn.! With input debug lof will be spammed with XY coordinates! */
	UPROPERTY()
	bool bDebugMouseInput;

	// Begin APlayerController Interface
	virtual void InitInputSystem() OVERRIDE;
	virtual void PlayerTick(float DeltaTime) OVERRIDE;
	// End APlayerController Interface

	/** Custom input to handle keyboard events with VaQuoleUI */
	void InputKeyQ(FViewport* Viewport, FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad);

	/** Helper to send events to VaQuoleUI components */
	// @TODO Make enum type blueprintable
	//UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void MouseEvent(EMouseEvent::Type MouseEvent, VaQuole::EMouseButton::Type Button);

	//////////////////////////////////////////////////////////////////////////
	// Mouse input example

	/** Left mouse button pressed */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void MouseDown();

	/** Left mouse button released */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void MouseUp();

	//////////////////////////////////////////////////////////////////////////
	// Web functions

	/** JS code will be passed directly to web view */
	UFUNCTION(exec)
	void EvalJS(const FString& ScriptSource);
	

protected:
	//////////////////////////////////////////////////////////////////////////
	// Internal helpers

	/** Fast HUD casting */
	AVaQuoleHUD* GetVaQuoleHUD() const;

	/** Mouse screen position helper */
	bool GetMouseScreenPosition(FVector2D& MousePosition);

	/** Get UI component we're aiming to */
	bool GetTargetUIComponent(FVector& ActorImpact, UVaQuoleUIComponent** View, UStaticMeshComponent** Mesh);

};
