// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

UVaQuoleUIComponent::UVaQuoleUIComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP), RefCount(new FThreadSafeCounter)
{
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	bool bResizeRequested = false;
	float LastResizeRequestTime = 0.0f;

	bHUD = false;
	bTransparent = false;

	Width = 256;
	Height = 256;

	DefaultURL = "http://alyamkin.com";

	TextureParameterName = TEXT("VaQuoleUITexture");
}

void UVaQuoleUIComponent::InitializeComponent()
{
	Super::InitializeComponent();

	RefCount->Increment();
	UE_LOG(LogVaQuole, Warning, TEXT("VaQuoleComponentUI # %d created"), RefCount->GetValue());

	// Init QApplication if we haven't one
	VaQuole::Init();

	// Create web view
	UIWidget = MakeShareable(new VaQuole::VaQuoleUI());

	// Init texture for the first time 
	SetTransparent(bTransparent);

	// Load resolution settings to set right HUD texture size
	if (bHUD)
	{
		auto GameUserSettings = GEngine->GetGameUserSettings();
		if (GameUserSettings)
		{
			FIntPoint CurrentResolution = GameUserSettings->GetScreenResolution();

			Width = CurrentResolution.X;
			Height = CurrentResolution.Y;
		}
		else
		{
			UE_LOG(LogVaQuole, Warning, TEXT("Can't get user settings to adjust HUD texture size"));
		}
	}
	
	// Resize texture to correspond desired size
	Resize(Width, Height);

	// Open default URL
	OpenURL(DefaultURL);
}

void UVaQuoleUIComponent::BeginDestroy()
{
	// Clear web view widget
	if (UIWidget.IsValid())
	{
		UIWidget->Destroy();
		UIWidget.Reset();
	}

	// Stop qApp if it was the last one
	if (RefCount->Decrement() == 0)
	{
		UE_LOG(LogVaQuole, Log, TEXT("Last VaQuole component being deleted, stop qApp now"));

		delete RefCount;
	}

	DestroyUITexture();

	Super::BeginDestroy();
}

void UVaQuoleUIComponent::ResetUITexture()
{
	DestroyUITexture();

	Texture = UTexture2D::CreateTransient(Width,Height);
	Texture->AddToRoot();
	Texture->UpdateResource();

	ResetMaterialInstance();
}

void UVaQuoleUIComponent::DestroyUITexture()
{
	if (Texture)
	{
		Texture->RemoveFromRoot();

		if (Texture->Resource)
		{
			BeginReleaseResource(Texture->Resource);

			FlushRenderingCommands();
		}

		Texture->MarkPendingKill();
		Texture = nullptr;
	}
}

void UVaQuoleUIComponent::ResetMaterialInstance()
{
	if (bHUD || !Texture || !BaseMaterial || TextureParameterName.IsNone())
	{
		return;
	}

	// Create material instance
	MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, NULL);
	if (!MaterialInstance)
	{
		UE_LOG(LogVaQuole, Warning, TEXT("UI Material instance can't be created"));
		return;
	}

	// Check we have desired parameter
	UTexture* Tex = nullptr;
	if (!MaterialInstance->GetTextureParameterValue(TextureParameterName, Tex))
	{
		UE_LOG(LogVaQuole, Warning, TEXT("UI Material instance Texture parameter not found"));
		return;
	}

	MaterialInstance->SetTextureParameterValue(TextureParameterName, GetTexture());
}

void UVaQuoleUIComponent::Resize(int32 NewWidth, int32 NewHeight)
{
	Width = NewWidth;
	Height = NewHeight;

	if (UIWidget.IsValid())
	{
		UIWidget->Resize(Width, Height);
	}

	ResetUITexture();
}

void UVaQuoleUIComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// HUD should be scaled to fit screen
	if (bHUD)
	{
		auto GameUserSettings = GEngine->GetGameUserSettings();
		if (GameUserSettings)
		{
			FIntPoint CurrentResolution = GameUserSettings->GetScreenResolution();

			if (Width != CurrentResolution.X || Height != CurrentResolution.Y)
			{
				Resize(CurrentResolution.X, CurrentResolution.Y);
			}
		}
		else
		{
			UE_LOG(LogVaQuole, Warning, TEXT("Can't get user settings to adjust HUD texture size"));
		}
	}
	
	// Redraw UI texture with current widget state
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
// View control

void UVaQuoleUIComponent::SetTransparent(bool Transparent)
{
	bTransparent = Transparent;

	if (UIWidget.IsValid())
	{
		UIWidget->SetTransparent(bTransparent);
	}
}

void UVaQuoleUIComponent::Redraw() const
{
	if (Texture && Texture->Resource && UIWidget.IsValid())
	{
		// Check that texture is prepared
		auto rhiRef = static_cast<FTexture2DResource*>(Texture->Resource)->GetTexture2DRHI();
		if (!rhiRef)
			return;

		// Load data from view
		const UCHAR* my_data = UIWidget->GrabView();
		const size_t size = Width * Height * sizeof(uint32);

		// Copy buffer for rendering thread
		TArray<uint32> ViewBuffer;
		ViewBuffer.Init(Width * Height);
		FMemory::Memcpy(ViewBuffer.GetData(), my_data, size);

		// Constuct buffer storage
		FVaQuoleTextureDataPtr DataPtr = MakeShareable(new FVaQuoleTextureData);
		DataPtr->SetRawData(Width, Height, sizeof(uint32), ViewBuffer);

		// Cleanup
		ViewBuffer.Empty();
		my_data = 0;

		ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(
			UpdateVaQuoleTexture,
			FVaQuoleTextureDataPtr, ImageData, DataPtr,
			FTexture2DRHIRef, TargetTexture, rhiRef,
			const size_t, DataSize, size,
			{
				uint32 stride = 0;
				void* MipData = GDynamicRHI->RHILockTexture2D(TargetTexture, 0, RLM_WriteOnly, stride, false);

				if (MipData)
				{
					FMemory::Memcpy(MipData, ImageData->GetRawBytesPtr(), ImageData->GetDataSize());
					GDynamicRHI->RHIUnlockTexture2D(TargetTexture, 0, false);
				}

				ImageData.Reset();
			});
	}
}


//////////////////////////////////////////////////////////////////////////
// Content control

void UVaQuoleUIComponent::OpenURL(const FString& URL)
{
	if (UIWidget.IsValid())
	{
		UIWidget->OpenURL(*URL);
	}
}


//////////////////////////////////////////////////////////////////////////
// Content access

UTexture2D* UVaQuoleUIComponent::GetTexture() const
{
	check(Texture);

	return Texture;
}

UMaterialInstanceDynamic* UVaQuoleUIComponent::GetMaterialInstance() const
{
	if (bHUD)
	{
		return nullptr;
	}

	check(MaterialInstance);

	return MaterialInstance;
}
