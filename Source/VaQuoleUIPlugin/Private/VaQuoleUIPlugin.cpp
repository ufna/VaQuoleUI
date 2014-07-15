// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

class FVaQuoleUIPlugin : public IVaQuoleUIPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() OVERRIDE
	{
		// @HACK Force classes to be compiled on shipping build
		UVaQuoleUIViewportClient::StaticClass();
		UVaQuoleBlueprintFunctionLibrary::StaticClass();
		UVaQuoleUIComponent::StaticClass();
		UVaQuoleHUDComponent::StaticClass();
		UVaQuoleSceneUIComponent::StaticClass();

		// Start QApplication thread
		VaQuole::Init();
	}

	virtual void ShutdownModule() OVERRIDE
	{
		// Exit QApplication
		VaQuole::Cleanup();
	}
};

IMPLEMENT_MODULE( FVaQuoleUIPlugin, VaQuoleUIPlugin )

DEFINE_LOG_CATEGORY(LogVaQuole);
