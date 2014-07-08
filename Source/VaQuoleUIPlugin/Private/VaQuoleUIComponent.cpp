// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

UVaQuoleUIComponent::UVaQuoleUIComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	WebUI = NULL;

	bEnabled = true;
	bTransparent = true;

	bConsumeMouseInput = false;
	bConsumeKeyboardInput = false;

	Width = 256;
	Height = 256;

	DefaultURL = "http://html5test.com";

	TextureParameterName = TEXT("VaQuoleUITexture");
}

void UVaQuoleUIComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Create web view
	if (!WebUI)
	{
		WebUI = VaQuole::ConstructNewUI();
	}

	// Init WebUI for the first time
	ResetWebUI();
}

void UVaQuoleUIComponent::BeginDestroy()
{
	// Clear web view widget
	if (WebUI)
	{
		WebUI->Destroy();
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
	if (!bEnabled || WebUI == NULL)
	{
		return;
	}

	// Don't update when WebView resizes or changes texture format
	if (WebUI->IsPendingVisualEvents())
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
		const UCHAR* my_data = WebUI->GrabView();
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

void UVaQuoleUIComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ResetWebUI();
}

void UVaQuoleUIComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Redraw UI texture with current widget state
	UpdateUITexture();

	// Mouse move is updated each frame
	UpdateMousePosition();

	// Process JS callback commands
	UpdateScriptResults();
	UpdateScriptEvents();
}

void UVaQuoleUIComponent::UpdateScriptResults()
{
	if (!WebUI)
	{
		return;
	}

	std::vector<VaQuole::ScriptEval> Evals;
	WebUI->GetScriptResults(Evals);

	// Process results only if we're enabled
	// Cached commands were cleared in GetScriptResults function
	if (bEnabled)
	{
		for (auto Eval : Evals)
		{
			FString ScriptUuid = Eval.ScriptUuid;
			FString ScriptResult = Eval.ScriptResult;

			ScriptEvalResult.Broadcast(ScriptUuid, ScriptResult);
		}
	}
}

void UVaQuoleUIComponent::UpdateScriptEvents()
{
	if (!WebUI)
	{
		return;
	}

	std::vector<VaQuole::ScriptEvent> Events;
	WebUI->GetScriptEvents(Events);

	// Process results only if we're enabled
	// Cached commands were cleared in GetScriptResults function
	if (bEnabled)
	{
		for (auto Event : Events)
		{
			FString EventName = Event.EventName;
			FString EventMessage = Event.EventMessage;

			ScriptEvent.Broadcast(EventName, EventMessage);
		}
	}
}

void UVaQuoleUIComponent::ResetWebUI()
{
	// Update enabled state
	SetEnabled(bEnabled);

	// Update transparency state
	SetTransparent(bTransparent);

	// Resize texture to correspond desired size
	Resize(Width, Height);

	// Open default URL
	OpenURL(DefaultURL);
}


//////////////////////////////////////////////////////////////////////////
// View control

void UVaQuoleUIComponent::SetEnabled(bool Enabled)
{
	bEnabled = Enabled;

	if (WebUI)
	{
		WebUI->SetEnabled(bEnabled);
	}
}

void UVaQuoleUIComponent::SetTransparent(bool Transparent)
{
	bTransparent = Transparent;

	if (WebUI)
	{
		WebUI->SetTransparent(bTransparent);
	}
}

void UVaQuoleUIComponent::SetConsumeMouseInput(const bool ConsumeInput)
{
	bConsumeMouseInput = ConsumeInput;
}

void UVaQuoleUIComponent::SetConsumeKeyboardInput(const bool ConsumeInput)
{
	bConsumeKeyboardInput = ConsumeInput;
}

void UVaQuoleUIComponent::Resize(int32 NewWidth, int32 NewHeight)
{
	Width = NewWidth;
	Height = NewHeight;

	if (WebUI)
	{
		WebUI->Resize(Width, Height);
	}

	ResetUITexture();
}

FString UVaQuoleUIComponent::EvaluateJavaScript(const FString& ScriptSource)
{
	if (!bEnabled || WebUI == NULL)
	{
		return TEXT("");
	}

	return WebUI->EvaluateJavaScript(*ScriptSource);
}

void UVaQuoleUIComponent::OpenURL(const FString& URL)
{
	if (!bEnabled || WebUI == NULL)
	{
		return;
	}

	if (URL.Contains(TEXT("vaquole://"), ESearchCase::IgnoreCase, ESearchDir::FromStart))
	{
		FString GameDir = FPaths::ConvertRelativePathToFull(FPaths::GameDir());
		FString LocalFile = URL.Replace(TEXT("vaquole://"), *GameDir, ESearchCase::IgnoreCase);
		LocalFile = FString(TEXT("file:///")) + LocalFile;

		UE_LOG(LogVaQuole, Log, TEXT("VaQuole opens %s"), *LocalFile);

		WebUI->OpenURL(*LocalFile);
	}
	else
	{
		WebUI->OpenURL(*URL);
	}
}


//////////////////////////////////////////////////////////////////////////
// Content access

bool UVaQuoleUIComponent::IsEnabled() const
{
	return bEnabled;
}

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
	if (!bEnabled || WebUI == NULL || !Key.IsValid())
	{
		return false;
	}

	// Check modifiers
	VaQuole::KeyModifiers Modifiers;
	Modifiers.bShiftDown = Viewport->KeyState(EKeys::LeftShift) || Viewport->KeyState(EKeys::RightShift);
	Modifiers.bCtrlDown = Viewport->KeyState(EKeys::LeftControl) || Viewport->KeyState(EKeys::RightControl);
	Modifiers.bAltDown = Viewport->KeyState(EKeys::LeftAlt) || Viewport->KeyState(EKeys::RightAlt);

	if (Key.IsMouseButton())
	{
		VaQuole::EMouseButton::Type MouseButton = VaQuole::EMouseButton::NoButton;

		if (Key == EKeys::MouseScrollUp)
		{
			MouseButton = VaQuole::EMouseButton::ScrollUp;
		}
		else if (Key == EKeys::MouseScrollDown)
		{
			MouseButton = VaQuole::EMouseButton::ScrollDown;
		}
		else if (Key == EKeys::LeftMouseButton)
		{
			MouseButton = VaQuole::EMouseButton::LeftButton;
		}
		else if (Key == EKeys::RightMouseButton)
		{
			MouseButton = VaQuole::EMouseButton::RightButton;
		}
		else if (Key == EKeys::MiddleMouseButton)
		{
			MouseButton = VaQuole::EMouseButton::MiddleButton;
		}
		else if (Key == EKeys::ThumbMouseButton)
		{
			MouseButton = VaQuole::EMouseButton::XButton1;
		}
		else if (Key == EKeys::ThumbMouseButton2)
		{
			MouseButton = VaQuole::EMouseButton::XButton2;
		}

		switch (EventType)
		{
		case IE_Pressed:
			WebUI->InputMouse((int32)MouseWidgetPosition.X, (int32)MouseWidgetPosition.Y, MouseButton, true, Modifiers);
			break;

		case IE_Released:
			WebUI->InputMouse((int32)MouseWidgetPosition.X, (int32)MouseWidgetPosition.Y, MouseButton, false, Modifiers);
			break;

		default:
			break;
		}

		// Don't consume IE_Released events to process action cancellation properly
		return (EventType == IE_Released) ? false : bConsumeMouseInput;
	}
	else if (Key.IsModifierKey())
	{
		// Modifiers are ignored here because we're using their values from Viewport
		return false;
	}
	else
	{
		const uint16 *KeyCode = 0;
		const uint16 *CharCode = 0;
		FInputKeyManager::Get().GetCodesFromKey(Key, KeyCode, CharCode);

		// Mark non-unicode characters with -1
		int32 KeyCodeVal = (KeyCode != NULL) ? *KeyCode : -1;

		// Send event
		switch (EventType)
		{
		case IE_Pressed:
			WebUI->InputKey(*Key.ToString(), KeyCodeVal, true, Modifiers);
			break;
		case IE_Released:
			WebUI->InputKey(*Key.ToString(), KeyCodeVal, false, Modifiers);
			break;
		case IE_Repeat:
			WebUI->InputKey(*Key.ToString(), KeyCodeVal, true, Modifiers);
			break;
		default:
			break;
		}

		// Don't consume IE_Released events to process action cancellation properly
		return (EventType == IE_Released) ? false : bConsumeKeyboardInput;
	}

	return false;
}

void UVaQuoleUIComponent::SetMousePosition(float X, float Y)
{
	MouseWidgetPosition = FVector2D(X, Y);
}

void UVaQuoleUIComponent::UpdateMousePosition()
{
	if (!bEnabled || WebUI == NULL)
	{
		return;
	}

	WebUI->InputMouse((int32)MouseWidgetPosition.X, (int32)MouseWidgetPosition.Y);
}


//////////////////////////////////////////////////////////////////////////
// Input helpers

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
