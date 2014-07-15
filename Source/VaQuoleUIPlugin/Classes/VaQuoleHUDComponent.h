// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleHUDComponent.generated.h"

/**
 * Class that handles view of one web page
 */
UCLASS(ClassGroup=UI, editinlinenew, meta=(BlueprintSpawnableComponent))
class VAQUOLEUIPLUGIN_API UVaQuoleHUDComponent : public UVaQuoleUIComponent
{
	GENERATED_UCLASS_BODY()

	// Begin UActorComponent Interface
	virtual void InitializeComponent() OVERRIDE;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) OVERRIDE;
	// End UActorComponent Interface

	// Begin UObject Interface
	virtual void BeginDestroy() OVERRIDE;
	// End UObject Interface

	// Begin UVaQuoleUIComponent Interface
	virtual void Resize(int32 NewWidth, int32 NewHeight) OVERRIDE;
	// End UVaQuoleUIComponent Interface

	//////////////////////////////////////////////////////////////////////////
	// View configuration

	/** Screen XY position */
	UPROPERTY(EditAnywhere, Category = "HUD")
	FVector2D ScreenPosition;

	/** Screen rectangle size */
	UPROPERTY(EditAnywhere, Category = "HUD")
	FVector2D ScreenSize;


	//////////////////////////////////////////////////////////////////////////
	// View control

	/** Current screen position of HUD widget */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|HUD")
	FVector2D GetScreenPosition() const;

	/** Set screen position to process mouse events for HUD widgets properly */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|HUD")
	void SetScreenPosition(float ScreenX, float ScreenY);

	/** Current screen size of HUD widget */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|HUD")
	FVector2D GetScreenSize() const;

	/** Set screen size to process mouse events for HUD widgets properly */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|HUD")
	void SetScreenSize(float ScreenWidth, float ScreenHeight);

};
