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

enum KeyboardModifier {
	NoModifier           = 0x00000000,
	ShiftModifier        = 0x02000000,
	ControlModifier      = 0x04000000,
	AltModifier          = 0x08000000,
	MetaModifier         = 0x10000000,
	KeypadModifier       = 0x20000000,
	GroupSwitchModifier  = 0x40000000,
	// Do not extend the mask to include 0x01000000
	KeyboardModifierMask = 0xfe000000
};

enum MouseButton {
	NoButton         = 0x00000000,
	LeftButton       = 0x00000001,
	RightButton      = 0x00000002,
	MidButton        = 0x00000004, // ### Qt 6: remove me
	MiddleButton     = MidButton,
	BackButton       = 0x00000008,
	XButton1         = BackButton,
	ExtraButton1     = XButton1,
	ForwardButton    = 0x00000010,
	XButton2         = ForwardButton,
	ExtraButton2     = ForwardButton,
	TaskButton       = 0x00000020,
	ExtraButton3     = TaskButton,
	ExtraButton4     = 0x00000040,
	ExtraButton5     = 0x00000080,
	ExtraButton6     = 0x00000100,
	ExtraButton7     = 0x00000200,
	ExtraButton8     = 0x00000400,
	ExtraButton9     = 0x00000800,
	ExtraButton10    = 0x00001000,
	ExtraButton11    = 0x00002000,
	ExtraButton12    = 0x00004000,
	ExtraButton13    = 0x00008000,
	ExtraButton14    = 0x00010000,
	ExtraButton15    = 0x00020000,
	ExtraButton16    = 0x00040000,
	ExtraButton17    = 0x00080000,
	ExtraButton18    = 0x00100000,
	ExtraButton19    = 0x00200000,
	ExtraButton20    = 0x00400000,
	ExtraButton21    = 0x00800000,
	ExtraButton22    = 0x01000000,
	ExtraButton23    = 0x02000000,
	ExtraButton24    = 0x04000000,
	AllButtons       = 0x07ffffff,
	MaxMouseButton   = ExtraButton24,
	// 4 high-order bits remain available for future use (0x08000000 through 0x40000000).
	MouseButtonMask  = 0xffffffff
};

} // namespace VaQuole

#endif // VAQUOLEPUBLICPCH_H
