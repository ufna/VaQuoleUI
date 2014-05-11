// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUILib.h"

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

VaQuoleWebView::VaQuoleWebView()
{
	CachedImg = 0;

	View = new QWebView();
	View->resize(256,256);
	//View->show();
}

void VaQuoleWebView::Destroy()
{
	if(View)
	{
		delete View;
	}
}

void VaQuoleWebView::OpenURL(const TCHAR* NewURL)
{
	QString Str = QString::fromUtf16((const ushort*)NewURL);
	View->load(QUrl(Str));
}

const uchar * VaQuoleWebView::GrabViewC()
{
	const QWidget::RenderFlags renderFlags = QWidget::DrawChildren;

	CachedImg = new QImage(View->width(),View->height(),QImage::Format_ARGB32);
	CachedImg->fill(QColor(0,0,0,0));

	QPainter p;
	p.begin(CachedImg);
	View->render(&p, QPoint(0,0),QRegion(CachedImg->rect()), renderFlags);
	p.end();

	return CachedImg->constBits();
}

uchar * VaQuoleWebView::GrabView()
{
	const QWidget::RenderFlags renderFlags = QWidget::DrawChildren;

	CachedImg = new QImage(View->width(),View->height(),QImage::Format_ARGB32);
	CachedImg->fill(QColor(0,0,0,0));

	QPainter p;
	p.begin(CachedImg);
	View->render(&p, QPoint(0,0),QRegion(CachedImg->rect()), renderFlags);
	p.end();

	return CachedImg->bits();
}

void VaQuoleWebView::ClearView()
{
	if(CachedImg)
	{
		delete CachedImg;
		CachedImg = 0;
	}
}

void VaQuoleWebView::Resize(int w, int h)
{
	View->resize(w,h);
}

} // namespace VaQuole
