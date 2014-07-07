// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUILib.h"
#include "VaQuoleAppThread.h"

#include <QApplication>
#include <QDebug>

#include <QImage>
#include <QWebFrame>
#include <QString>
#include <QHash>

namespace VaQuole
{

/** Main app thread with QApplication */
static VaQuoleUIManager* pAppThread = NULL;

/** Key map to convert UE4 keys to Qt ones */
QHash<QString, Qt::Key> KeyMap;

/** Key map to convert UE4 keys to QString characters */
QHash<QString, QString> KeyTextMap;

/**
 * Key map to apply SHIFT modificator. Seemed as Qt bug,
 * Check https://bugreports.qt-project.org/browse/QTBUG-40100
 */
QHash<QString, QString> KeyShiftMap;


//////////////////////////////////////////////////////////////////////////
// Common lib functions

void Init()
{
	// Check that we haven't qApp already
	if (QApplication::instance() == nullptr)
	{
		pAppThread = new VaQuoleUIManager();
		pAppThread->start();
	}

	InitKeyMaps();
}

void Cleanup()
{
	qDebug() << "Clean so clean";

	if (pAppThread != NULL)
	{
		pAppThread->~VaQuoleUIManager();//->stop();
	}
}

VaQuoleWebUI* ConstructNewUI()
{
	VaQuoleWebUI* NewUI = new VaQuoleWebUI();
	NewUI->Register();

	return NewUI;
}

void InitKeyMaps()
{
	KeyMap.clear();

	KeyMap.insert("BackSpace", Qt::Key_Backspace);
	KeyMap.insert("Tab", Qt::Key_Tab);
	KeyMap.insert("Enter", Qt::Key_Enter);
	KeyMap.insert("Pause", Qt::Key_Pause);

	KeyMap.insert("CapsLock", Qt::Key_CapsLock);
	KeyMap.insert("Escape", Qt::Key_Escape);
	KeyMap.insert("SpaceBar", Qt::Key_Space);
	KeyMap.insert("PageUp", Qt::Key_PageUp);
	KeyMap.insert("PageDown", Qt::Key_PageDown);
	KeyMap.insert("End", Qt::Key_End);
	KeyMap.insert("Home", Qt::Key_Home);

	KeyMap.insert("Left", Qt::Key_Left);
	KeyMap.insert("Up", Qt::Key_Up);
	KeyMap.insert("Right", Qt::Key_Right);
	KeyMap.insert("Down", Qt::Key_Down);
	KeyMap.insert("Insert", Qt::Key_Insert);
	KeyMap.insert("Delete", Qt::Key_Delete);

	KeyMap.insert("NumPadZero", Qt::Key_0);
	KeyMap.insert("NumPadOne", Qt::Key_1);
	KeyMap.insert("NumPadTwo", Qt::Key_2);
	KeyMap.insert("NumPadThree", Qt::Key_3);
	KeyMap.insert("NumPadFour", Qt::Key_4);
	KeyMap.insert("NumPadFive", Qt::Key_5);
	KeyMap.insert("NumPadSix", Qt::Key_6);
	KeyMap.insert("NumPadSeven", Qt::Key_7);
	KeyMap.insert("NumPadEight", Qt::Key_8);
	KeyMap.insert("NumPadNine", Qt::Key_9);

	/* Will be processed as text values (see below)
	KeyMap.insert("Multiply", Qt::Key_multiply);
	KeyMap.insert("Add", Qt::Key_Plus);
	KeyMap.insert("Subtract", Qt::Key_Minus);
	KeyMap.insert("Decimal", Qt::Key_Percent);
	KeyMap.insert("Divide", Qt::Key_division);*/

	KeyMap.insert("F1", Qt::Key_F1);
	KeyMap.insert("F2", Qt::Key_F2);
	KeyMap.insert("F3", Qt::Key_F3);
	KeyMap.insert("F4", Qt::Key_F4);
	KeyMap.insert("F5", Qt::Key_F5);
	KeyMap.insert("F6", Qt::Key_F6);
	KeyMap.insert("F7", Qt::Key_F7);
	KeyMap.insert("F8", Qt::Key_F8);
	KeyMap.insert("F9", Qt::Key_F9);
	KeyMap.insert("F10", Qt::Key_F10);
	KeyMap.insert("F11", Qt::Key_F11);
	KeyMap.insert("F12", Qt::Key_F12);

	KeyMap.insert("NumLock", Qt::Key_NumLock);
	KeyMap.insert("ScrollLock", Qt::Key_ScrollLock);

	KeyMap.insert("LeftShift", Qt::Key_Shift);
	KeyMap.insert("RightShift", Qt::Key_Shift);
	KeyMap.insert("LeftControl", Qt::Key_Control);
	KeyMap.insert("RightControl", Qt::Key_Control);
	KeyMap.insert("LeftAlt", Qt::Key_Alt);
	KeyMap.insert("RightAlt", Qt::Key_Alt);
	KeyMap.insert("LeftCommand", Qt::Key_Meta);
	KeyMap.insert("RightCommand", Qt::Key_Meta);

	/* Will be processed as text values (see below)
	KeyMap.insert("Semicolon", Qt::Key_Semicolon);
	KeyMap.insert("Equals", Qt::Key_Equal);
	KeyMap.insert("Comma", Qt::Key_Comma);
	KeyMap.insert("Underscore", Qt::Key_Underscore);
	KeyMap.insert("Period", Qt::Key_Period);
	KeyMap.insert("Slash", Qt::Key_Slash);
	KeyMap.insert("Tilde", Qt::Key_AsciiTilde);
	KeyMap.insert("LeftBracket", Qt::Key_BracketLeft);
	KeyMap.insert("Backslash", Qt::Key_Backslash);
	KeyMap.insert("RightBracket", Qt::Key_BracketRight);
	KeyMap.insert("Quote", Qt::Key_QuoteDbl);*/


	//--------------------------------------------------------------
	KeyTextMap.clear();

	KeyTextMap.insert("Zero", "0");
	KeyTextMap.insert("One", "1");
	KeyTextMap.insert("Two", "2");
	KeyTextMap.insert("Three", "3");
	KeyTextMap.insert("Four", "4");
	KeyTextMap.insert("Five", "5");
	KeyTextMap.insert("Six", "6");
	KeyTextMap.insert("Seven", "7");
	KeyTextMap.insert("Eight", "8");
	KeyTextMap.insert("Nine", "9");

	KeyTextMap.insert("NumPadZero", "0");
	KeyTextMap.insert("NumPadOne", "1");
	KeyTextMap.insert("NumPadTwo", "2");
	KeyTextMap.insert("NumPadThree", "3");
	KeyTextMap.insert("NumPadFour", "4");
	KeyTextMap.insert("NumPadFive", "5");
	KeyTextMap.insert("NumPadSix", "6");
	KeyTextMap.insert("NumPadSeven", "7");
	KeyTextMap.insert("NumPadEight", "8");
	KeyTextMap.insert("NumPadNine", "9");

	KeyTextMap.insert("Multiply", "*");
	KeyTextMap.insert("Add", "+");
	KeyTextMap.insert("Subtract", "-");
	KeyTextMap.insert("Decimal", "%");
	KeyTextMap.insert("Divide", "/");

	KeyTextMap.insert("Semicolon", ";");
	KeyTextMap.insert("Equals", "=");
	KeyTextMap.insert("Comma", ",");
	KeyTextMap.insert("Underscore", "_");
	KeyTextMap.insert("Period", ".");
	KeyTextMap.insert("Slash", "/");
	KeyTextMap.insert("Tilde", "`");
	KeyTextMap.insert("LeftBracket", "[");
	KeyTextMap.insert("Backslash", "\\");
	KeyTextMap.insert("RightBracket", "]");
	KeyTextMap.insert("Quote", "'");


	//--------------------------------------------------------------
	KeyShiftMap.clear();

	KeyShiftMap.insert("0", ")");
	KeyShiftMap.insert("1", "!");
	KeyShiftMap.insert("2", "@");
	KeyShiftMap.insert("3", "#");
	KeyShiftMap.insert("4", "$");
	KeyShiftMap.insert("5", "%");
	KeyShiftMap.insert("6", "^");
	KeyShiftMap.insert("7", "&");
	KeyShiftMap.insert("8", "*");
	KeyShiftMap.insert("9", "(");

	KeyShiftMap.insert(";", ":");
	KeyShiftMap.insert("=", "+");
	KeyShiftMap.insert(".", ">");
	KeyShiftMap.insert("-", "_");
	KeyShiftMap.insert(",", "<");
	KeyShiftMap.insert("/", "?");
	KeyShiftMap.insert("`", "~");
	KeyShiftMap.insert("[", "{");
	KeyShiftMap.insert("\\", "|");
	KeyShiftMap.insert("]", "}");
	KeyShiftMap.insert("'", "\"");
}


//////////////////////////////////////////////////////////////////////////
// WebPage class

VaQuoleWebUI::VaQuoleWebUI()
{
	ExtComm = new UIDataKeeper;
}

void VaQuoleWebUI::Destroy()
{
	std::lock_guard<std::mutex> guard(mutex);

	Q_CHECK_PTR(ExtComm);
	ExtComm->bMarkedForDelete = true;
}

UIDataKeeper* VaQuoleWebUI::GetData()
{
	Q_CHECK_PTR(ExtComm);
	return ExtComm;
}

/** Register UI page in global UI manager */
void VaQuoleWebUI::Register()
{
	Q_CHECK_PTR(pAppThread);

	pAppThread->AddPage(this);
}

void VaQuoleWebUI::OpenURL(const TCHAR* NewURL)
{
	std::lock_guard<std::mutex> guard(mutex);

	Q_CHECK_PTR(ExtComm);
	ExtComm->NewURL = QString::fromUtf16((const ushort*)NewURL);
}

void VaQuoleWebUI::OpenBenchmark()
{
	OpenURL(L"http://www.smashcat.org/av/canvas_test/");
}

void VaQuoleWebUI::EvaluateJavaScript(const TCHAR *ScriptSource)
{
	return;
	Q_CHECK_PTR(ExtComm);

	//QString Str = QString::fromUtf16((const ushort*)ScriptSource);
	//WebView->page()->mainFrame()->evaluateJavaScript(Str);
}

const uchar * VaQuoleWebUI::GrabView()
{
	std::lock_guard<std::mutex> guard(mutex);

	Q_CHECK_PTR(ExtComm);
	return ExtComm->ImageBits;
}

void VaQuoleWebUI::SetTransparent(bool Transparent)
{
	std::lock_guard<std::mutex> guard(mutex);

	Q_CHECK_PTR(ExtComm);
	ExtComm->bDesiredTransparency = Transparent;
}

void VaQuoleWebUI::Resize(int w, int h)
{
	std::lock_guard<std::mutex> guard(mutex);

	Q_CHECK_PTR(ExtComm);
	ExtComm->DesiredWidth = w;
	ExtComm->DesiredHeight = h;
}

bool VaQuoleWebUI::IsPendingVisualEvents()
{
	std::lock_guard<std::mutex> guard(mutex);

	Q_CHECK_PTR(ExtComm);
	bool bPendingTransparency = ExtComm->bTransparent != ExtComm->bDesiredTransparency;
	bool bPendingSize = (ExtComm->Width != ExtComm->DesiredWidth) || (ExtComm->Height != ExtComm->DesiredHeight);

	return (bPendingTransparency || bPendingSize);
}


//////////////////////////////////////////////////////////////////////////
// JS commands callback

int VaQuoleWebUI::GetCachedCommandsNumber()
{
	return 0;
	Q_CHECK_PTR(ExtComm);

	//return WebView->getCachedCommandsNumber();
}

TCHAR * VaQuoleWebUI::GetCachedCommand(int Index)
{
	return NULL;
	Q_CHECK_PTR(ExtComm);

	//return (TCHAR *)WebView->getCachedCommand(Index).utf16();
}

void VaQuoleWebUI::ClearCachedCommands()
{
	return;
	Q_CHECK_PTR(ExtComm);

	//WebView->clearCachedCommands();
}


//////////////////////////////////////////////////////////////////////////
// Player input

void VaQuoleWebUI::InputMouse(int X, int Y, VaQuole::EMouseButton::Type Button,
								bool bMouseDown,
								const VaQuole::KeyModifiers Modifiers)
{
	std::lock_guard<std::mutex> guard(mutex);

	MouseEvent Event;
	Event.eventPos = QPoint(X,Y);
	Event.bButtonPressed = bMouseDown;

	// Prepare modifiers
	Modifiers.bAltDown == true ? Event.modifiers |= Qt::AltModifier : 0;
	Modifiers.bCtrlDown == true ? Event.modifiers |= Qt::ControlModifier : 0;
	Modifiers.bShiftDown == true ? Event.modifiers |= Qt::ShiftModifier : 0;

	// Convert button from UE4 to Qt enum value
	switch (Button)
	{
	case VaQuole::EMouseButton::NoButton:
		Event.button = Qt::NoButton;
		break;

	case VaQuole::EMouseButton::LeftButton:
		Event.button = Qt::LeftButton;
		break;

	case VaQuole::EMouseButton::RightButton:
		Event.button = Qt::RightButton;
		break;

	case VaQuole::EMouseButton::MiddleButton:
		Event.button = Qt::MiddleButton;
		break;

	case VaQuole::EMouseButton::XButton1:
		Event.button = Qt::XButton1;
		break;

	case VaQuole::EMouseButton::XButton2:
		Event.button = Qt::XButton2;
		break;

	case VaQuole::EMouseButton::ScrollUp:
		break;

	case VaQuole::EMouseButton::ScrollDown:
		break;

	default:
		Event.button = Qt::NoButton;
		break;
	}

	Q_CHECK_PTR(ExtComm);
	ExtComm->MouseEvents.append(Event);
}

void VaQuoleWebUI::InputKey(const TCHAR *Key,
							const unsigned int KeyCode,
							const bool bPressed,
							const VaQuole::KeyModifiers Modifiers)
{
	std::lock_guard<std::mutex> guard(mutex);

	KeyEvent Event;
	Event.bKeyPressed = bPressed;

	// Prepare modifiers
	Modifiers.bAltDown == true ? Event.modifiers |= Qt::AltModifier : 0;
	Modifiers.bCtrlDown == true ? Event.modifiers |= Qt::ControlModifier : 0;
	Modifiers.bShiftDown == true ? Event.modifiers |= Qt::ShiftModifier : 0;

	// Prepare key
	QString KeyStr = QString::fromUtf16((const ushort*)Key);
	Event.key = KeyMap.value(KeyStr, Qt::Key_unknown);
	Event.text = KeyTextMap.value(KeyStr);

	// Give a chance for UE4 to determine the character
	if(Event.key == Qt::Key_unknown && KeyCode != -1)
	{
		Event.key = (Qt::Key)KeyCode;
	}

	// [1] Qt bug temporary solution
	// https://bugreports.qt-project.org/browse/QTBUG-40100
	if(Event.text.isEmpty())
	{
		Event.text = QString(QChar(Event.key));
	}

	// [2] Qt bug temporary solution
	// Apply shift modificator
	if( Modifiers.bShiftDown )
	{
		QString ShiftedStr = KeyShiftMap.value(Event.text);
		if(!ShiftedStr.isNull())
		{
			Event.text = ShiftedStr;
		}
	}
	else
	{
		Event.text = Event.text.toLower();
		qDebug() << Event.text;
	}

	// Apply keypad modifier for numpad data
	if (KeyStr.contains("NumPad", Qt::CaseInsensitive))
	{
		Event.modifiers |= Qt::KeypadModifier;
	}

	Q_CHECK_PTR(ExtComm);
	ExtComm->KeyEvents.append(Event);
}

} // namespace VaQuole
