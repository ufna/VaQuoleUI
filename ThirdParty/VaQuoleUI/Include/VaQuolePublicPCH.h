// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEPUBLICPCH_H
#define VAQUOLEPUBLICPCH_H

/** Some defines missed in QtCreator */
typedef unsigned char uchar;

#ifdef UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif

namespace VaQuole
{

/**
 * Available mouse buttons enum
 */
namespace EMouseButton
{
	enum Type
	{
		NoButton,
		LeftButton,
		RightButton,
		MiddleButton,
		XButton1,
		XButton2,
		// Scroll will be processed as a mouse button too
		ScrollUp,
		ScrollDown
	};
}

/**
 * Holds set of modifiers applied to key
 */
struct KeyModifiers
{
	bool bShiftDown;
	bool bCtrlDown;
	bool bAltDown;

	/** Defaults **/
	KeyModifiers::KeyModifiers()
	{
		bShiftDown = false;
		bCtrlDown = false;
		bAltDown = false;
	}
};

/**
 * Simple struct to keep return values from JavaScript calls
 */
struct ScriptEval
{
	TCHAR* ScriptUuid;
	TCHAR* ScriptResult;
};

/**
 * JavaScript events keeper
 */
struct ScriptEvent
{
	TCHAR* EventName;
	TCHAR* EventMessage;
};

} // namespace VaQuole

#endif // VAQUOLEPUBLICPCH_H
