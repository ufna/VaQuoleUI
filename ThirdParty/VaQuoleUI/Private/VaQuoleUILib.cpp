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
	// Check that we haven't qApp already
	if (QApplication::instance() == nullptr)
	{
		// Create qApp
		int argc = 1;
		char arg1[] = "VaQuoleUILib";
		char* argv[1] = {arg1};
		pApp = new QApplication(argc, argv);
		pApp->setQuitOnLastWindowClosed(false);
		pApp->processEvents();

		// Set network config
		QNetworkProxyFactory::setUseSystemConfiguration (true);
		QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
		QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);
	}
}

void Update()
{
	if(QApplication::instance())
	{
		QApplication::instance()->processEvents();
	}
}

void Cleanup()
{
	// First process unfinished events if we have them
	Update();

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
	OpenURL(L"http://google.com");
	//OpenURL(L"http://www.smashcat.org/av/canvas_test/");
}

const uchar * VaQuoleUI::GrabView()
{
	Q_CHECK_PTR(ViewThread);

	return ViewThread->grabView();
}

void VaQuoleUI::Resize(int w, int h)
{
	Q_CHECK_PTR(ViewThread);

	ViewThread->resize(w,h);
}

} // namespace VaQuole
