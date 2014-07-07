// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleUIComponent.generated.h"

/**
 * Holds texture data for upload to a rendering resource
 */
struct FVaQuoleTextureData
{
	FVaQuoleTextureData(uint32 InWidth = 0, uint32 InHeight = 0, uint32 InStride = 0, const TArray<uint32>& InBytes = TArray<uint32>())
		: Bytes(InBytes)
		, Width(InWidth)
		, Height(InHeight)
		, StrideBytes(InStride)
	{

	}

	FVaQuoleTextureData(const FVaQuoleTextureData &Other)
		: Bytes(Other.Bytes)
		, Width(Other.Width)
		, Height(Other.Height)
		, StrideBytes(Other.StrideBytes)
	{

	}

	FVaQuoleTextureData& operator=(const FVaQuoleTextureData& Other)
	{
		if (this != &Other)
		{
			SetRawData(Other.Width, Other.Height, Other.StrideBytes, Other.Bytes);
		}
		return *this;
	}

	void SetRawData(uint32 InWidth, uint32 InHeight, uint32 InStride, const TArray<uint32>& InBytes)
	{
		Width = InWidth;
		Height = InHeight;
		StrideBytes = InStride;
		Bytes = InBytes;
	}

	void Empty()
	{
		Bytes.Empty();
	}

	uint32 GetWidth() const { return Width; }
	uint32 GetHeight() const { return Height; }
	uint32 GetStride() const { return StrideBytes; }
	uint32 GetDataSize() const { return Width * Height * StrideBytes; }
	const TArray<uint32>& GetRawBytes() const { return Bytes; }

	/** Accesses the raw bytes of already sized texture data */
	uint32* GetRawBytesPtr() { return Bytes.GetTypedData(); }

private:
	/** Raw uncompressed texture data */
	TArray<uint32> Bytes;

	/** Width of the texture */
	uint32 Width;

	/** Height of the texture */
	uint32 Height;

	/** The number of bytes of each pixel */
	uint32 StrideBytes;

};

typedef TSharedPtr<FVaQuoleTextureData, ESPMode::ThreadSafe> FVaQuoleTextureDataPtr;
typedef TSharedRef<FVaQuoleTextureData, ESPMode::ThreadSafe> FVaQuoleTextureDataRef;

/**
 * Class that handles view of one web page
 */
UCLASS(ClassGroup=UI, editinlinenew, meta=(BlueprintSpawnableComponent))
class UVaQuoleUIComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	// Begin UActorComponent Interface
	virtual void InitializeComponent() OVERRIDE;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) OVERRIDE;
	// End UActorComponent Interface

	// Begin UObject Interface
	virtual void BeginDestroy() OVERRIDE;
	// End UObject Interface


	//////////////////////////////////////////////////////////////////////////
	// View configuration

	/** Indicates whether the View enabled (receive player input or not) */
	UPROPERTY(EditAnywhere, Category = "View")
	bool bEnabled;

	/** Indicates whether the View is transparent or composed on white */
	UPROPERTY(EditAnywhere, Category = "View")
	bool bTransparent;

	/** Width of target texture */
	UPROPERTY(EditAnywhere, Category = "View", meta = (ClampMin = "0", UIMin = "0", UIMax = "4096"))
	int32 Width;

	/** Height of target texture */
	UPROPERTY(EditAnywhere, Category = "View", meta = (ClampMin = "0", UIMin = "0", UIMax = "4096"))
	int32 Height;

	/** URL that will be opened on startup */
	UPROPERTY(EditAnywhere, Category = "View")
	FString DefaultURL;

	/** Should widget consume mouse input? (mouse events won't be delivered to other actors) */
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bConsumeMouseInput;

	/** Should widget consume keyboard input? (key events won't be delivered to other actors) */
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bConsumeKeyboardInput;

	/** Material that will be instanced to load UI texture into it */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* BaseMaterial;

	/** Name of parameter to load UI texture into material */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	FName TextureParameterName;


	//////////////////////////////////////////////////////////////////////////
	// View control

	/** Changes view activity state. Attn.! qApp will be still active in background! */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void SetEnabled(const bool Enabled);

	/** Changes background transparency */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void SetTransparent(const bool Transparent);

	/** Set consume input state for mouse events */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|Input")
	void SetConsumeMouseInput(const bool ConsumeInput);

	/** Set consume input state for keyboard events */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|Input")
	void SetConsumeKeyboardInput(const bool ConsumeInput);

	/** Resizes the View */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	virtual void Resize(int32 NewWidth, int32 NewHeight);

	/** JS code will be passed directly to web view */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void EvaluateJavaScript(const FString& ScriptSource);

	/** Requests a new URL to be loaded in the View */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	void OpenURL(const FString& URL);


	//////////////////////////////////////////////////////////////////////////
	// Content access

	/** Is current view enabled? */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	bool IsEnabled() const;

	/** Texture that stores current widget UI */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	int32 GetWidth() const;

	/** Texture that stores current widget UI */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	int32 GetHeight() const;

	/** Texture that stores current widget UI */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	UTexture2D* GetTexture() const;

	/** Material instance that contains texture inside it */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI")
	UMaterialInstanceDynamic* GetMaterialInstance() const;


	//////////////////////////////////////////////////////////////////////////
	// Player input

	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed = 1.f, bool bGamepad = false);

	/** Set new mouse position (relative to widget!) */
	void SetMousePosition(float X, float Y);

protected:
	/** Send MouseMove event to the widget */
	void UpdateMousePosition();

	/** Cached mouse position */
	FVector2D MouseWidgetPosition;


	//////////////////////////////////////////////////////////////////////////
	// Materials setup

protected:
	/** Release Texture resources */
	void DestroyUITexture();

	/** Release current Texture and create new with desired size */
	void ResetUITexture();

	/** Material instance update */
	void ResetMaterialInstance();
	
	/** Update Texture with WebView cached data */
	void UpdateUITexture();

	/** Texture that stores current widget UI */
	UTexture2D* Texture;

	/** Material instance that contains texture inside it */
	UMaterialInstanceDynamic* MaterialInstance;

	/** Web view loaded from library */
	VaQuole::VaQuoleWebUI* WebUI;


	//////////////////////////////////////////////////////////////////////////
	// Input helpers

	/** Player mouse screen position */
	bool GetMouseScreenPosition(FVector2D& MousePosition);

};
