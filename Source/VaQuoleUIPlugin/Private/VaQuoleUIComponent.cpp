// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

UVaQuoleUIComponent::UVaQuoleUIComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP), RefCount(new FThreadSafeCounter)
{
	//bAutoActivate = false;
	PrimaryComponentTick.bCanEverTick = true;

	RefCount->Increment();
	UE_LOG(LogVaQuole, Warning, TEXT("VaQuoleComponentUI # %d created"), RefCount->GetValue());

	// Init texture for the first time
	Resize(512, 512);

	// Init QApplication if we haven't one
	VaQuole::Init();

	// Create web view
	UIWidget = MakeShareable(new VaQuole::VaQuoleWebView());
}

void UVaQuoleUIComponent::BeginDestroy()
{
	if (RefCount->Decrement() == 0)
	{
		delete RefCount;
		UE_LOG(LogVaQuole, Log, TEXT("Last VaQuole component being deleted, stop qApp now"));
		
		// Stop qApp
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

	// Process Qt events
	VaQuole::Update();

	// Redraw UI texture with current widget state
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
// View control

void UVaQuoleUIComponent::Resize(int32 NewWidth, int32 NewHeight)
{
	Width = NewWidth;
	Height = NewHeight;

	ResetUITexture();
}

void UVaQuoleUIComponent::Redraw() const
{
	if (Texture && Texture->Resource && UIWidget.IsValid())
	{
		/*const UCHAR* my_data = UIWidget->GrabScreenD();

		//UE_LOG(LogOcean, Warning, TEXT("Check RHI"));

		auto rhiRef = static_cast<FTexture2DResource*>(Texture->Resource)->GetTexture2DRHI();
		if (!rhiRef)
			return;

		//UE_LOG(LogOcean, Warning, TEXT("Go render"));

		const size_t size = 256 * 256 * 4;
		ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(
			UpdateTextureRHI,
			const UCHAR*, SourceMemory, my_data,
			FTexture2DRHIRef, ModifTexture, rhiRef,
			const size_t, MemSize, size,
			{
			uint32 stride = 0;
			void* MipData = GDynamicRHI->RHILockTexture2D(ModifTexture, 0, RLM_WriteOnly, stride, false);
			if (MipData)
			{
				FMemory::Memcpy(MipData, SourceMemory, MemSize);
				GDynamicRHI->RHIUnlockTexture2D(ModifTexture, 0, false);
			}
		});*/
	}
}


//////////////////////////////////////////////////////////////////////////
// Content control

void UVaQuoleUIComponent::OpenURL(const FString& URL)
{

}


//////////////////////////////////////////////////////////////////////////
// Content access

UTexture2D* UVaQuoleUIComponent::GetTexture() const
{
	check(Texture);

	return Texture;
}
