// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleUIPlugin.generated.inl"

class FVaQuoleUIPlugin : public IVaQuoleUIPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() OVERRIDE
	{
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
