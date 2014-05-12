// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUILib.h"
#include "VaQuoleWebView.h"

#include <QApplication>
#include <QNetworkProxyFactory>
#include <QWebSettings>
#include <QWebView>

#include <QPainter>
#include <QPixmap>
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

		// Set network config
		QNetworkProxyFactory::setUseSystemConfiguration (true);
		QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
		QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);
	}
}

void Update()
{
	if(pApp)
	{
		pApp->processEvents();
	}
}

void Cleanup()
{
	// First process unfinished events if we have them
	Update();

	if(pApp)
	{
		pApp->quit();
		delete pApp;
		pApp = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
// WebView class

VaQuoleUI::VaQuoleUI()
{
	CachedImg = 0;

	View = new VaQuoleWebView();
	View->resize(256,256);
	View->show();
}

void VaQuoleUI::Destroy()
{
	if(View)
	{
		delete View;
	}
}

void VaQuoleUI::OpenURL(const TCHAR* NewURL)
{
	QString Str = QString::fromUtf16((const ushort*)NewURL);
	View->load(QUrl(Str));
}

void VaQuoleUI::OpenBenchmark()
{
	OpenURL(L"http://www.smashcat.org/av/canvas_test/");
}

const uchar * VaQuoleUI::GrabViewC()
{
	return View->ImageCache.constBits();
}

uchar * VaQuoleUI::GrabView()
{
	return View->ImageCache.bits();
}

void VaQuoleUI::ClearView()
{
	if(CachedImg)
	{
		delete CachedImg;
		CachedImg = 0;
	}
}

void VaQuoleUI::Resize(int w, int h)
{
	View->resize(w,h);
}

} // namespace VaQuole
