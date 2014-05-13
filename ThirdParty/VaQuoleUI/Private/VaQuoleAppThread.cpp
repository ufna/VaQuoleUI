// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleAppThread.h"
#include "VaQuoleWebView.h"

#include <QApplication>
#include <QNetworkProxyFactory>
#include <QWebSettings>
#include <QWebView>

VaQuoleAppThread::VaQuoleAppThread(QObject *parent) :
	QThread(parent)
{
	View = 0;
	pApp = 0;

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

VaQuoleAppThread::~VaQuoleAppThread()
{

}

void VaQuoleAppThread::createView(int w, int h)
{
	// Check we have a view already
	closeView();

	View = new VaQuoleWebView();
	View->moveToThread(this);
	View->resize(w,h);
	View->show();
}

void VaQuoleAppThread::closeView()
{
	if(View)
	{
		View->close();
		delete View;
		View = 0;
	}
}


//////////////////////////////////////////////////////////////////////////
// Thread control

void VaQuoleAppThread::stopThread()
{
	closeView();

	quit();
	wait();
}

void VaQuoleAppThread::run()
{
	View->moveToThread(this);

	if (pApp)
	{
		pApp->exec();
	}
}


//////////////////////////////////////////////////////////////////////////
// VaQuole API

void VaQuoleAppThread::openURL(const QString& NewURL)
{
	Q_CHECK_PTR(View);

	View->load(QUrl(NewURL));
}

const uchar * VaQuoleAppThread::grabView()
{
	Q_CHECK_PTR(View);

	return View->getImageData();
}

void VaQuoleAppThread::setTransparent(bool transparent)
{
	Q_CHECK_PTR(View);

	View->setTransparent(transparent);
}

void VaQuoleAppThread::resize(int w, int h)
{
	Q_CHECK_PTR(View);

	View->resize(w,h);
}
