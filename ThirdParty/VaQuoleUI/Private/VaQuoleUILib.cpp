// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUILib.h"
#include "VaQuoleWebView.h"
#include "VaQuoleAppThread.h"

#include <QApplication>
#include <QNetworkProxyFactory>
#include <QWebSettings>

#include <QImage>

namespace VaQuole
{

/** Main Qt class object */
static QApplication* pApp = NULL;

//////////////////////////////////////////////////////////////////////////
// Common lib functions

void Init()
{
	// @TODO I pretty sure I can delete it
	return;
}

void Update()
{
	if (QApplication::instance() != nullptr)
	{
		QApplication::instance()->processEvents();
	}
}

void Cleanup()
{
	if (QApplication::instance() != nullptr)
	{
		QApplication::instance()->quit();
	}
}


//////////////////////////////////////////////////////////////////////////
// WebView class

VaQuoleUI::VaQuoleUI()
{
	ViewThread = new VaQuoleAppThread();
	ViewThread->createView();
	ViewThread->start();
}

void VaQuoleUI::Destroy()
{
	if(ViewThread)
	{
		ViewThread->stopThread();
	}
}

void VaQuoleUI::OpenURL(const TCHAR* NewURL)
{
	Q_CHECK_PTR(ViewThread);

	QString Str = QString::fromUtf16((const ushort*)NewURL);
	ViewThread->openURL(Str);
}

void VaQuoleUI::OpenBenchmark()
{
	OpenURL(L"http://html5test.com");
	//OpenURL(L"http://www.smashcat.org/av/canvas_test/");
}

const uchar * VaQuoleUI::GrabView()
{
	Q_CHECK_PTR(ViewThread);

	return ViewThread->grabView();
}

void VaQuoleUI::SetTransparent(bool transparent)
{
	Q_CHECK_PTR(ViewThread);

	ViewThread->setTransparent(transparent);
}

void VaQuoleUI::Resize(int w, int h)
{
	Q_CHECK_PTR(ViewThread);

	ViewThread->resize(w,h);
}

//////////////////////////////////////////////////////////////////////////
// Player input

void VaQuoleUI::MouseMove(int x, int y)
{
	Q_CHECK_PTR(ViewThread);

	ViewThread->mouseMove(x,y);
}

void VaQuoleUI::MouseClick(int x, int y, VaQuole::MouseButton button,
						   bool bPressed, unsigned int modifiers)
{
	Q_CHECK_PTR(ViewThread);

	ViewThread->mouseClick(x, y, button, bPressed, modifiers);
}

} // namespace VaQuole
