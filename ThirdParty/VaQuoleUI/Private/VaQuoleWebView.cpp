// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleWebView.h"
#include "VaQuoleInputHelpers.h"

#include <QWebFrame>
#include <QPaintEvent>

VaQuoleWebView::VaQuoleWebView(QWidget *parent) :
	QWebView(parent)
{
	// It should never be empty
	ImageCache = QImage(32,32,QImage::Format_RGB32);

#ifndef VA_DEBUG
	// Hide window in taskbar
	setWindowFlags(Qt::SplashScreen);
#endif
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
		ImageCache = QImage(ImageSize, QImage::Format_RGB32);
		ImageCache.fill(Qt::white);
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
	return ImageCache.bits();
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
	p.begin(&ImageCache);
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
