// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

UVaQuoleUIComponent::UVaQuoleUIComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	WebPage = NULL;

	bEnabled = true;
	bTransparent = true;

	bSceneUI = false;
	SceneInputDistanceMax = 400;

	Width = 256;
	Height = 256;

	DefaultURL = "http://html5test.com";

	TextureParameterName = TEXT("VaQuoleUITexture");
}

void UVaQuoleUIComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Create web view
	WebPage = VaQuole::ConstructNewPage();

	// Init texture for the first time 
	SetTransparent(bTransparent);
	
	// Resize texture to correspond desired size
	Resize(Width, Height);

	// Open default URL
	OpenURL(DefaultURL);

	// Register page with Viewport Client to receive input
	if (!bSceneUI && GEngine && GEngine->GameViewport)
	{
		UVaQuoleUIViewportClient* ViewportClient = Cast<UVaQuoleUIViewportClient>(GEngine->GameViewport);
		if (ViewportClient)
		{
			ViewportClient->RegisterWebUI(this);
		}
	}
}

void UVaQuoleUIComponent::BeginDestroy()
{
	// Unregister page from Viewport
	if (!bSceneUI && GEngine && GEngine->GameViewport)
	{
		UVaQuoleUIViewportClient* ViewportClient = Cast<UVaQuoleUIViewportClient>(GEngine->GameViewport);
		if (ViewportClient)
		{
			ViewportClient->UnregisterWebUI(this);
		}
	}

	// Clear web view widget
	if (WebPage)
	{
		WebPage->Destroy();
	}

	DestroyUITexture();

	Super::BeginDestroy();
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

void UVaQuoleUIComponent::ResetUITexture()
{
	DestroyUITexture();

	Texture = UTexture2D::CreateTransient(Width,Height);
	Texture->AddToRoot();
	Texture->UpdateResource();

	ResetMaterialInstance();
}

void UVaQuoleUIComponent::ResetMaterialInstance()
{
	if (!Texture || !BaseMaterial || TextureParameterName.IsNone())
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

void UVaQuoleUIComponent::UpdateUITexture()
{
	// Ignore texture update
	if (!bEnabled || WebPage == NULL)
	{
		return;
	}

	// Don't update when WebView resizes or changes texture format
	if (WebPage->IsPendingVisualEvents())
	{
		return;
	}

	if (Texture && Texture->Resource)
	{
		// Check that texture is prepared
		auto rhiRef = static_cast<FTexture2DResource*>(Texture->Resource)->GetTexture2DRHI();
		if (!rhiRef)
			return;

		// Load data from view
		const UCHAR* my_data = WebPage->GrabView();
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

void UVaQuoleUIComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Process JS callback commands (currenly HUD only)
	/*if (UIWidget.IsValid())
	{
		AHUD* MyHUD = Cast<AHUD>(GetOwner());
		APlayerController* const PlayerController = MyHUD ? MyHUD->PlayerOwner : NULL;

		// It will work only for players
		if (PlayerController)
		{
			int32 Amount = UIWidget->GetCachedCommandsNumber();
			FString Command;

			for (int i = 0; i < Amount; i++)
			{
				Command = UIWidget->GetCachedCommand(i);

				if (!Command.IsEmpty())
				{
					PlayerController->ConsoleCommand(Command);
				}
			}
		}

		// Attn.! It's neccessary to prevent commands spam!
		UIWidget->ClearCachedCommands();
	}*/
	
	// Redraw UI texture with current widget state
	UpdateUITexture();
}


//////////////////////////////////////////////////////////////////////////
// View control

void UVaQuoleUIComponent::SetEnabled(bool Enabled)
{
	bEnabled = Enabled;
}

void UVaQuoleUIComponent::SetTransparent(bool Transparent)
{
	bTransparent = Transparent;

	if (WebPage)
	{
		WebPage->SetTransparent(bTransparent);
	}
}

void UVaQuoleUIComponent::Resize(int32 NewWidth, int32 NewHeight)
{
	Width = NewWidth;
	Height = NewHeight;

	if (WebPage)
	{
		WebPage->Resize(Width, Height);
	}

	ResetUITexture();
}

void UVaQuoleUIComponent::EvaluateJavaScript(const FString& ScriptSource)
{
	if (!bEnabled || WebPage == NULL)
	{
		return;
	}

	WebPage->EvaluateJavaScript(*ScriptSource);
}

void UVaQuoleUIComponent::OpenURL(const FString& URL)
{
	if (!bEnabled || WebPage == NULL)
	{
		return;
	}

	if (URL.Contains(TEXT("vaquole://"), ESearchCase::IgnoreCase, ESearchDir::FromStart))
	{
		FString GameDir = FPaths::ConvertRelativePathToFull(FPaths::GameDir());
		FString LocalFile = URL.Replace(TEXT("vaquole://"), *GameDir, ESearchCase::IgnoreCase);
		LocalFile = FString(TEXT("file:///")) + LocalFile;

		UE_LOG(LogVaQuole, Log, TEXT("VaQuole opens %s"), *LocalFile);

		WebPage->OpenURL(*LocalFile);
	}
	else
	{
		WebPage->OpenURL(*URL);
	}
}


//////////////////////////////////////////////////////////////////////////
// Content access

int32 UVaQuoleUIComponent::GetWidth() const
{
	return Width;
}

int32 UVaQuoleUIComponent::GetHeight() const
{
	return Height;
}

UTexture2D* UVaQuoleUIComponent::GetTexture() const
{
	check(Texture);

	return Texture;
}

UMaterialInstanceDynamic* UVaQuoleUIComponent::GetMaterialInstance() const
{
	return MaterialInstance;
}


//////////////////////////////////////////////////////////////////////////
// Player input

bool UVaQuoleUIComponent::InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	if (!bEnabled || WebPage == NULL || !Key.IsValid())
	{
		return false;
	}

	// Check modifiers
	bool bShiftDown = Viewport->KeyState(EKeys::LeftShift) || Viewport->KeyState(EKeys::RightShift);
	bool bCtrlDown = Viewport->KeyState(EKeys::LeftControl) || Viewport->KeyState(EKeys::RightControl);
	bool bAltDown = Viewport->KeyState(EKeys::LeftAlt) || Viewport->KeyState(EKeys::RightAlt);

	if (Key.IsMouseButton())
	{
		if (Key == EKeys::MouseScrollUp)
		{

		}
		else if (Key == EKeys::MouseScrollDown)
		{

		}
		else if (Key == EKeys::LeftMouseButton)
		{

		}
		else if (Key == EKeys::RightMouseButton)
		{

		}
		else if (Key == EKeys::MiddleMouseButton)
		{

		}
		else if (Key == EKeys::ThumbMouseButton)
		{

		}
		else if (Key == EKeys::ThumbMouseButton2)
		{

		}

		UE_LOG(LogVaQuole, Warning, TEXT("Views: %d"), (int)EventType);
		// @TODO Process mouse button
	}
	else if (Key.IsModifierKey())
	{

	}
	else
	{
		// Check extra key codes
		/*uint32 KeyCode = 0x20;
		if (Key == EKeys::BackSpace)
		{
			KeyCode = 0x01000003;
		}
		else
		{
			KeyCode = GetKeyCodeFromKey(Key);
		}*/

		// Send event
		/*switch (EventType)
		{
		case IE_Pressed:
			WebPage->InputKey(KeyCode, true, Modifiers);
			break;
		case IE_Released:
			WebPage->InputKey(KeyCode, false, Modifiers);
			break;
		case IE_Repeat:
			WebPage->InputKey(KeyCode, true, Modifiers);
			break;
		case IE_DoubleClick:
			break;
		case IE_Axis:
			break;
		case IE_MAX:
			break;
		default:
			break;
		}*/
	}

	return false;
}

void UVaQuoleUIComponent::MouseMove(int32 X, int32 Y)
{
	if (!bEnabled || WebPage == NULL)
	{
		return;
	}

	WebPage->MouseMove(X, Y);
}

void UVaQuoleUIComponent::MouseClick(int32 X, int32 Y, VaQuole::EMouseButton::Type Button, bool bPressed, VaQuole::KeyModifiers Modifiers)
{
	if (!bEnabled || WebPage == NULL)
	{
		return;
	}

	WebPage->MouseClick(X, Y, Button, bPressed, Modifiers);
}

bool UVaQuoleUIComponent::GetMouseScreenPosition(FVector2D& MousePosition)
{
#if PLATFORM_DESKTOP
	if (GEngine && GEngine->GameViewport)
	{
		MousePosition = GEngine->GameViewport->GetMousePosition();
		return true;
	}
#endif	

	return false;
}
