// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleUIComponent.generated.h"

/**
 * Class that handles view of one web page
 */
UCLASS(ClassGroup=UI, editinlinenew, meta=(BlueprintSpawnableComponent))
class UVaQuoleUIComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////
	// View configuration

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|VaQuole", meta = (ClampMin = "0", UIMin = "0", UIMax = "4096"))
	int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|VaQuole", meta = (ClampMin = "0", UIMin = "0", UIMax = "4096"))
	int32 Height;


	//////////////////////////////////////////////////////////////////////////
	// View control

	/** Resizes the View */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void Resize(int32 NewWidth, int32 NewHeight);

	/** Requests a View to completely re-draw itself */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void Redraw() const;


	//////////////////////////////////////////////////////////////////////////
	// Content control

	/** Requests a new URL to be loaded in the View */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void OpenURL(const FString& URL);


	//////////////////////////////////////////////////////////////////////////
	// Content access

	/** Texture that stores current widget UI */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	UTexture2D* GetTexture() const;
	


public:
	// Begin UActorComponent Interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) OVERRIDE;
	// End UActorComponent Interface

	// Begin UObject Interface
	virtual void BeginDestroy() OVERRIDE;
	// End UObject Interface

protected:
	void ResetUITexture();
	void DestroyUITexture();

	/** Texture that stores current widget UI */
	UTexture2D* Texture;

	/** Web view loaded from library */
	TSharedPtr<class VaQuole::VaQuoleWebView> UIWidget;

private:
	/** Counter to control life of qApp */
	FThreadSafeCounter* RefCount;

};
