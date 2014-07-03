// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUILib.h"
#include "VaQuoleAppThread.h"

#include "VaQuoleWebView.h"
#include "VaQuoleInputHelpers.h"

#include <QApplication>

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
	if (pAppThread != nullptr)
	{
		pAppThread->stop();
	}
}

VaQuoleWebPage* ConstructNewPage()
{
	VaQuoleWebPage* NewUI = new VaQuoleWebPage();
	NewUI->Register();

	return NewUI;
}


//////////////////////////////////////////////////////////////////////////
// WebPage class

VaQuoleWebPage::VaQuoleWebPage()
{
	ExtComm = new UIDataKeeper;
}

void VaQuoleWebPage::Destroy()
{
	// @TODO Mark for deletion
}

UIDataKeeper* VaQuoleWebPage::GetData()
{
	return ExtComm;
}

/** Register UI page in global UI manager */
void VaQuoleWebPage::Register()
{
	Q_CHECK_PTR(pAppThread);

	pAppThread->AddPage(this);
}

void VaQuoleWebPage::OpenURL(const TCHAR* NewURL)
{
	std::lock_guard<std::mutex> guard(mutex);

	Q_CHECK_PTR(ExtComm);
	ExtComm->NewURL = QString::fromUtf16((const ushort*)NewURL);
}

void VaQuoleWebPage::OpenBenchmark()
{
	OpenURL(L"http://www.smashcat.org/av/canvas_test/");
}

void VaQuoleWebPage::EvaluateJavaScript(const TCHAR *ScriptSource)
{
	return;
	Q_CHECK_PTR(ExtComm);

	//QString Str = QString::fromUtf16((const ushort*)ScriptSource);
	//WebView->page()->mainFrame()->evaluateJavaScript(Str);
}

const uchar * VaQuoleWebPage::GrabView()
{
	return NULL;
	Q_CHECK_PTR(ExtComm);

	//return WebView->getImageData();
}

void VaQuoleWebPage::SetTransparent(bool Transparent)
{
	std::lock_guard<std::mutex> guard(mutex);

	Q_CHECK_PTR(ExtComm);
	ExtComm->bTransparent = Transparent;
	ExtComm->bTransparencyChanged = true;
}

void VaQuoleWebPage::Resize(int w, int h)
{
	return;
	Q_CHECK_PTR(ExtComm);

	//WebView->resize(w,h);
}


//////////////////////////////////////////////////////////////////////////
// JS commands callback

int VaQuoleWebPage::GetCachedCommandsNumber()
{
	return 0;
	Q_CHECK_PTR(ExtComm);

	//return WebView->getCachedCommandsNumber();
}

TCHAR * VaQuoleWebPage::GetCachedCommand(int Index)
{
	return NULL;
	Q_CHECK_PTR(ExtComm);

	//return (TCHAR *)WebView->getCachedCommand(Index).utf16();
}

void VaQuoleWebPage::ClearCachedCommands()
{
	return;
	Q_CHECK_PTR(ExtComm);

	//WebView->clearCachedCommands();
}


//////////////////////////////////////////////////////////////////////////
// Player input

void VaQuoleWebPage::MouseMove(int x, int y)
{
	return;
	Q_CHECK_PTR(ExtComm);

	//VaQuole::simulateMouseMove(WebView, QPoint(x,y));
	//pApp->processEvents();
}

void VaQuoleWebPage::MouseClick(int x, int y, VaQuole::EMouseButton::Type button,
						   bool bPressed, unsigned int modifiers)
{
	return;
	Q_CHECK_PTR(ExtComm);

	/*VaQuole::simulateMouseClick(WebView, QPoint(x,y),
								(Qt::MouseButton) button,
								(Qt::KeyboardModifiers) modifiers,
								bPressed);*/
	//pApp->processEvents();
}

void VaQuoleWebPage::InputKey(const unsigned int key,
						 const bool bPressed,
						 const unsigned int modifiers)
{
	return;
	Q_CHECK_PTR(ExtComm);

	//VaQuole::simulateKey(WebView, key, (Qt::KeyboardModifiers) modifiers, bPressed);
	//pApp->processEvents();
}

} // namespace VaQuole
