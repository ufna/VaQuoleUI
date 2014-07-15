// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleSceneUIComponent.generated.h"

UENUM(BlueprintType)
namespace ESurfaceMapping
{
	enum Type
	{
		Planar,
		Cylindrical,
		// @TODO Spherical,
		// @TODO Box
	};
}

/**
 * Class that handles view of one web page
 */
UCLASS(ClassGroup=UI, editinlinenew, meta=(BlueprintSpawnableComponent))
class VAQUOLEUIPLUGIN_API UVaQuoleSceneUIComponent : public UVaQuoleUIComponent
{
	GENERATED_UCLASS_BODY()

	// Begin UObject Interface
	virtual void BeginDestroy() OVERRIDE;
	// End UObject Interface

	/** How surface we're projected too is mapped */
	UPROPERTY(EditAnywhere, Category = "View")
	TEnumAsByte<ESurfaceMapping::Type> SurfaceMapping;


	//////////////////////////////////////////////////////////////////////////
	// View control

	/** Is WebUI registered to receive player input? */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|SceneUI")
	bool IsInputRegistered();

	/** Register UI component to receive player input */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|SceneUI")
	void RegisterInput(bool bRegisterInput = true);

	/** Unregister UI component (it won't receive player input) */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|SceneUI")
	void UnregisterInput();

	/** Set new mouse position (relative to widget!) */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|SceneUI")
	void MouseMove(float X, float Y);

	/** Set mouse position from trace result */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|SceneUI")
	bool MouseMoveFromHitResult(const FHitResult& HitResult);


	//////////////////////////////////////////////////////////////////////////
	// Input control data
private:

	/** Being registered WebUI receives direct player input from VieportClient */
	bool bRegisteredUI;

};
