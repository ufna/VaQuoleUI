// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleUIPlugin.generated.inl"

class FVaQuoleUIPlugin : public IVaQuoleUIPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() OVERRIDE
	{

	}

	virtual void ShutdownModule() OVERRIDE
	{

	}
};

IMPLEMENT_MODULE( FVaQuoleUIPlugin, VaQuoleUIPlugin )

DEFINE_LOG_CATEGORY(LogVaQuole);
