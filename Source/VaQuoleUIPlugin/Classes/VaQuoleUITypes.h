// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUITypes.generated.h"

UENUM(BlueprintType)
namespace EMouseEvent
{
	enum Type
	{
		Move,
		Pressed,
		Released,
		Wheel,
		DoubleClick
	};
}

/** deprecated */
UENUM(BlueprintType)
namespace EKeyboardModifier
{
	enum Type
	{
		NoModifier,
		ShiftModifier,
		ControlModifier,
		AltModifier,
		MetaModifier,
		KeypadModifier,
		GroupSwitchModifier
	};
}
