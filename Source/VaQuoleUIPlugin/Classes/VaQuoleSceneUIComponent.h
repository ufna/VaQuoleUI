// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleSceneUIComponent.generated.h"

/**
 * Class that handles view of one web page
 */
UCLASS(ClassGroup=UI, editinlinenew, meta=(BlueprintSpawnableComponent))
class UVaQuoleSceneUIComponent : public UVaQuoleUIComponent
{
	GENERATED_UCLASS_BODY()

	// Begin UObject Interface
	virtual void BeginDestroy() OVERRIDE;
	// End UObject Interface


	//////////////////////////////////////////////////////////////////////////
	// View control

	/** Set new mouse position (relative to widget!) */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|SceneUI")
	void MouseMove(float X, float Y);

};
