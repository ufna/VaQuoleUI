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

	bEnabled = true;
	bHUD = true;
	bTransparent = true;

	Width = 256;
	Height = 256;

	DefaultURL = "http://html5test.com";

	TextureParameterName = TEXT("VaQuoleUITexture");
}

void UVaQuoleUIComponent::InitializeComponent()
{
	Super::InitializeComponent();

	RefCount->Increment();
	UE_LOG(LogVaQuole, Warning, TEXT("VaQuoleComponentUI # %d created"), RefCount->GetValue());

	// Init key map storage
	InitKeyMap();

	// Init QApplication if we haven't one
	VaQuole::Init();

	// Create web view
	UIWidget = MakeShareable(new VaQuole::VaQuoleUI());

	// Init texture for the first time 
	SetTransparent(bTransparent);
	
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

	// Process Qt events
	VaQuole::Update();

	// Process JS callback commands (currenly HUD only)
	if (UIWidget.IsValid())
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
				UE_LOG(LogVaQuole, Warning, TEXT("Ho-Ho-Hp %d"), Amount);
				Command = UIWidget->GetCachedCommand(i);

				if (!Command.IsEmpty())
				{
					PlayerController->ConsoleCommand(Command);
				}
			}
		}

		// Attn.! It's neccessary to prevent commands spam!
		UIWidget->ClearCachedCommands();
	}
	
	// Redraw UI texture with current widget state
	Redraw();
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

	if (UIWidget.IsValid())
	{
		UIWidget->SetTransparent(bTransparent);
	}
}

void UVaQuoleUIComponent::Redraw() const
{
	// Ignore texture update
	if (!bEnabled)
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

void UVaQuoleUIComponent::EvaluateJavaScript(const FString& ScriptSource)
{
	if (!bEnabled || !UIWidget.IsValid())
	{
		return;
	}

	UIWidget->EvaluateJavaScript(*ScriptSource);
}


//////////////////////////////////////////////////////////////////////////
// Player input

void UVaQuoleUIComponent::MouseMove(int32 X, int32 Y)
{
	if (!bEnabled || !UIWidget.IsValid())
	{
		return;
	}

	UIWidget->MouseMove(X, Y);
}

void UVaQuoleUIComponent::MouseClick(int32 X, int32 Y, VaQuole::EMouseButton::Type Button, bool bPressed, unsigned int Modifiers)
{
	if (!bEnabled || !UIWidget.IsValid())
	{
		return;
	}

	UIWidget->MouseClick(X, Y, Button, bPressed, Modifiers);
}

void UVaQuoleUIComponent::InputKeyQ(FViewport* Viewport, FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	if (!bEnabled || !UIWidget.IsValid() || !Key.IsValid())
	{
		return;
	}

	if (Key.IsMouseButton())
	{
		// @TODO Process mouse button
	}
	else if (Key.IsModifierKey())
	{
		
	}
	else
	{
		// Check modifiers
		bool bShiftDown = Viewport->KeyState(EKeys::LeftShift) || Viewport->KeyState(EKeys::RightShift);
		bool bCtrlDown = Viewport->KeyState(EKeys::LeftControl) || Viewport->KeyState(EKeys::RightControl);
		bool bAltDown = Viewport->KeyState(EKeys::LeftAlt) || Viewport->KeyState(EKeys::RightAlt);

		uint32 Modifiers = VaQuole::EKeyboardModifier::NoModifier;

		if (bShiftDown)
		{
			Modifiers |= VaQuole::EKeyboardModifier::ShiftModifier;
		}

		if (bCtrlDown)
		{
			Modifiers |= VaQuole::EKeyboardModifier::ControlModifier;
		}

		if (bAltDown)
		{
			Modifiers |= VaQuole::EKeyboardModifier::AltModifier;
		}

		// Check extra key codes
		uint32 KeyCode = 0x20;
		if (Key == EKeys::BackSpace)
		{
			KeyCode = 0x01000003;
		}
		else
		{
			KeyCode = GetKeyCodeFromKey(Key);
		}

		// Send event
		switch (EventType)
		{
		case IE_Pressed:
			UIWidget->InputKey(KeyCode, true, Modifiers);
			break;
		case IE_Released:
			UIWidget->InputKey(KeyCode, false, Modifiers);
			break;
		case IE_Repeat:
			UIWidget->InputKey(KeyCode, true, Modifiers);
			break;
		case IE_DoubleClick:
			break;
		case IE_Axis:
			break;
		case IE_MAX:
			break;
		default:
			break;
		}
	}
}

void UVaQuoleUIComponent::InitKeyMap()
{
	for (int i = 0; i <= MAX_int16; i++)
	{
		KeyMapEnumToCode.Add(FInputKeyManager::Get().GetKeyFromCodes(i, i), i);
	}
}

//////////////////////////////////////////////////////////////////////////
// Content control

void UVaQuoleUIComponent::OpenURL(const FString& URL)
{
	if (!bEnabled || UIWidget.IsValid())
	{
		if (URL.Contains(TEXT("vaquole://"), ESearchCase::IgnoreCase, ESearchDir::FromStart))
		{
			FString GameDir = FPaths::GameDir();
			FString LocalFile = URL.Replace(TEXT("vaquole://"), *GameDir, ESearchCase::IgnoreCase);
			LocalFile = FString(TEXT("file:///")) + LocalFile;

			UE_LOG(LogVaQuole, Log, TEXT("VaQuole opens %s"), *LocalFile);

			UIWidget->OpenURL(*LocalFile);
		}
		else
		{
			UIWidget->OpenURL(*URL);
		}
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
	if (bHUD)
	{
		return nullptr;
	}

	check(MaterialInstance);

	return MaterialInstance;
}

uint16 UVaQuoleUIComponent::GetKeyCodeFromKey(FKey& Key) const
{
	const uint16* Value = KeyMapEnumToCode.Find(Key);

	if (Value != NULL)
	{
		return *Value;
	}
	
	return 0;
}
