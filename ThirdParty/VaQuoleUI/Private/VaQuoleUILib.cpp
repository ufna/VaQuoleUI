// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUILib.h"
#include "VaQuoleWebView.h"
#include "VaQuoleInputHelpers.h"

#include <QApplication>
#include <QNetworkProxyFactory>
#include <QWebSettings>

#include <QImage>
#include <QWebFrame>

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
	else
	{
		pApp = qApp;
	}
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
	WebView = new VaQuoleWebView();
	WebView->show();
}

void VaQuoleUI::Destroy()
{
	if(WebView)
	{
		WebView->close();
		delete WebView;
		WebView = 0;
	}
}

void VaQuoleUI::OpenURL(const TCHAR* NewURL)
{
	Q_CHECK_PTR(WebView);

	QString Str = QString::fromUtf16((const ushort*)NewURL);
	WebView->load(QUrl(Str));
}

void VaQuoleUI::OpenBenchmark()
{
	OpenURL(L"http://www.smashcat.org/av/canvas_test/");
}

void VaQuoleUI::EvaluateJavaScript(const TCHAR *ScriptSource)
{
	Q_CHECK_PTR(WebView);

	QString Str = QString::fromUtf16((const ushort*)ScriptSource);
	WebView->page()->mainFrame()->evaluateJavaScript(Str);
}

const uchar * VaQuoleUI::GrabView()
{
	Q_CHECK_PTR(WebView);

	return WebView->getImageData();
}

void VaQuoleUI::SetTransparent(bool transparent)
{
	Q_CHECK_PTR(WebView);

	WebView->setTransparent(transparent);
}

void VaQuoleUI::Resize(int w, int h)
{
	Q_CHECK_PTR(WebView);

	WebView->resize(w,h);
}


//////////////////////////////////////////////////////////////////////////
// JS commands callback

int VaQuoleUI::GetCachedCommandsNumber()
{
	Q_CHECK_PTR(WebView);

	return WebView->getCachedCommandsNumber();
}

TCHAR * VaQuoleUI::GetCachedCommand(int Index)
{
	Q_CHECK_PTR(WebView);

	return (TCHAR *)WebView->getCachedCommand(Index).utf16();
}

void VaQuoleUI::ClearCachedCommands()
{
	Q_CHECK_PTR(WebView);

	WebView->clearCachedCommands();
}


//////////////////////////////////////////////////////////////////////////
// Player input

void VaQuoleUI::MouseMove(int x, int y)
{
	Q_CHECK_PTR(WebView);

	VaQuole::simulateMouseMove(WebView, QPoint(x,y));
	pApp->processEvents();
}

void VaQuoleUI::MouseClick(int x, int y, VaQuole::EMouseButton::Type button,
						   bool bPressed, unsigned int modifiers)
{
	Q_CHECK_PTR(WebView);

	VaQuole::simulateMouseClick(WebView, QPoint(x,y),
								(Qt::MouseButton) button,
								(Qt::KeyboardModifiers) modifiers,
								bPressed);
	pApp->processEvents();
}

void VaQuoleUI::InputKey(const unsigned int key,
						 const bool bPressed,
						 const unsigned int modifiers)
{
	Q_CHECK_PTR(WebView);

	VaQuole::simulateKey(WebView, key, (Qt::KeyboardModifiers) modifiers, bPressed);
	pApp->processEvents();
}

} // namespace VaQuole
