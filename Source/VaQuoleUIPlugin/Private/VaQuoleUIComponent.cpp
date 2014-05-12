// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

UVaQuoleUIComponent::UVaQuoleUIComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP), RefCount(new FThreadSafeCounter)
{
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	DefaultURL = "http://alyamkin.com";
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
	Resize(512,512);

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
		VaQuole::Cleanup();
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
}

void UVaQuoleUIComponent::DestroyUITexture()
{
	if (Texture)
	{
		Texture->RemoveFromRoot();

		if (Texture->Resource)
		{
			Texture->ReleaseResource();
		}

		Texture->MarkPendingKill();
		Texture = nullptr;
	}
}

void UVaQuoleUIComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Redraw UI texture with current widget state
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
// View control

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

void UVaQuoleUIComponent::Redraw() const
{
	// Minor timeout
	if (GetWorld()->GetTimeSeconds() < 1)
	{
		return;
	}

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
