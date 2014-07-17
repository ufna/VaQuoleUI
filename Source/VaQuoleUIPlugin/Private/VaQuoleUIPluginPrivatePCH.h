// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "CoreUObject.h"
#include "Engine.h"
#include "Engine/Console.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVaQuole, Log, All);

#include "IVaQuoleUIPlugin.h"

// Qt VaQuole core
#include "AllowWindowsPlatformTypes.h"
#include "VaQuolePublicPCH.h"
#include "VaQuoleUILib.h"
#include "HideWindowsPlatformTypes.h"

// Engine plugin implementation
#include "VaQuoleUIPluginClasses.h"
