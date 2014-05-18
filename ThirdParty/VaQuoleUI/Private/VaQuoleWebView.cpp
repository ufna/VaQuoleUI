// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleWebView.h"
#include "VaQuoleInputHelpers.h"

#include <QWebFrame>
#include <QPaintEvent>
#include <QBackingStore>

VaQuoleWebView::VaQuoleWebView(QWidget *parent) :
	QWebView(parent)
{
	// It should never be empty
	ImageCache = QImage(32,32,QImage::Format_RGB32);

#ifndef VA_DEBUG
	// Hide window in taskbar
	setWindowFlags(Qt::SplashScreen);
#endif

	// Register us with JavaScript
	connect(this, SIGNAL(loadFinished(bool)),
			this, SLOT(registerJavaScriptWindowObject(bool)));
}

void VaQuoleWebView::updateImageCache(QSize ImageSize)
{
	if(!ImageSize.isValid())
	{
		ImageSize = size();
	}

	if(bTransparent)
	{
		ImageCache = QImage(ImageSize, QImage::Format_ARGB32);
		ImageCache.fill(Qt::transparent);
	}
	else
	{
		ImageCache = QImage();
	}
}

//////////////////////////////////////////////////////////////////////////
// View control functions

void VaQuoleWebView::setTransparent(bool transparent)
{
	bTransparent = transparent;

	updateImageCache();

	if(bTransparent)
	{
		setStyleSheet("background-color: transparent;");
	}
	else
	{
		setStyleSheet("");
	}
}

void VaQuoleWebView::resize(int w, int h)
{
	QWebView::resize(w, h);

	// Recreate image cache
	updateImageCache(QSize(w,h));

#ifndef VA_DEBUG
	// Place our black empty widget out of screen
	move(-width(),-height());
#endif
}


//////////////////////////////////////////////////////////////////////////
// Data access

const uchar * VaQuoleWebView::getImageData()
{
	if (bTransparent)
	{
		return ImageCache.bits();
	}

	QImage *backBuffer = dynamic_cast<QImage*>(backingStore()->paintDevice());
	return backBuffer->bits();
}

const QStringList& VaQuoleWebView::getCachedCommands()
{
	return cachedJavaScriptCommands;
}

void VaQuoleWebView::clearCachedCommands()
{
	cachedJavaScriptCommands.clear();
}


//////////////////////////////////////////////////////////////////////////
// Qt Events

void VaQuoleWebView::paintEvent(QPaintEvent *ev)
{
	QWebPage *page = this->page();
	if (!page)
	{
		return;
	}

	QWebFrame *frame = page->mainFrame();
	QPainter p;

	if (bTransparent)
	{
		p.begin(&ImageCache);
	}
	else
	{
		p.begin(this);
	}

	p.setRenderHints(renderHints());

	if(bTransparent)
	{
		// Clear background of current rect
		p.setBackgroundMode(Qt::TransparentMode);
		p.setCompositionMode (QPainter::CompositionMode_Source);
		p.fillRect(ev->rect(), Qt::transparent);
		p.setCompositionMode (QPainter::CompositionMode_SourceOver);
	}

	frame->render(&p, ev->region());
	p.end();
}


//////////////////////////////////////////////////////////////////////////
// Page communication

void VaQuoleWebView::registerJavaScriptWindowObject(bool pageLoaded)
{
	if(pageLoaded)
	{
		page()->mainFrame()->addToJavaScriptWindowObject("VaQuoleUI", this);
	}
}

void VaQuoleWebView::scriptCommand(QString command)
{
	cachedJavaScriptCommands.append(command);
}
