// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUILib.h"
#include "VaQuoleAppThread.h"

#include <QApplication>
#include <QDebug>

#include <QImage>
#include <QWebFrame>
#include <QString>

namespace VaQuole
{

/** Main app thread with QApplication */
static VaQuoleUIManager* pAppThread = NULL;

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

	// Prepare modifiers
	Modifiers.bAltDown == true ? Event.modifiers |= Qt::AltModifier : 0;
	Modifiers.bCtrlDown == true ? Event.modifiers |= Qt::ControlModifier : 0;
	Modifiers.bShiftDown == true ? Event.modifiers |= Qt::ShiftModifier : 0;

	Q_CHECK_PTR(ExtComm);
	ExtComm->MouseEvents.append(Event);
}

void VaQuoleWebUI::InputKey(const TCHAR *Key,
							  const bool bPressed,
							  const VaQuole::KeyModifiers Modifiers)
{
	return;
	Q_CHECK_PTR(ExtComm);

	//VaQuole::simulateKey(WebView, key, (Qt::KeyboardModifiers) modifiers, bPressed);
	//pApp->processEvents();
}

} // namespace VaQuole
