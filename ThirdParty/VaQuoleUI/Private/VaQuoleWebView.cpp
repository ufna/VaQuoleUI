// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleWebView.h"
#include "VaQuoleInputHelpers.h"

#include <QWebFrame>
#include <QPaintEvent>
#include <QBackingStore>

namespace VaQuole
{

VaQuoleWebView::VaQuoleWebView(QWidget *parent) :
	QWebView(parent)
{
	// It should never be empty
	ImageCache = QImage(32,32,QImage::Format_RGB32);

	// Defaults
	bPageLoaded = false;

#ifndef VA_DEBUG
	// Hide window in taskbar
	setWindowFlags(Qt::SplashScreen);
#endif

	// Register us with JavaScript
	connect(this, SIGNAL(loadFinished(bool)), this, SLOT(registerJavaScriptWindowObject(bool)));
	connect(this, SIGNAL(loadFinished(bool)), this, SLOT(markLoadFinished(bool)));
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

void VaQuoleWebView::markLoadFinished(bool ok)
{
	// We just want to know is it finished or not, but not the result
	bPageLoaded = true;
}

//////////////////////////////////////////////////////////////////////////
// View control functions

bool VaQuoleWebView::getTransparency() const
{
	return bTransparent;
}

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

bool VaQuoleWebView::isLoadFinished() const
{
	return bPageLoaded;
}

void VaQuoleWebView::resetPageLoadState()
{
	bPageLoaded = false;
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

uchar *VaQuoleWebView::getImageData()
{
	if (bTransparent)
	{
		return ImageCache.bits();
	}

	QImage *backBuffer = dynamic_cast<QImage*>(backingStore()->paintDevice());
	return backBuffer->bits();
}

int VaQuoleWebView::getImageDataSize()
{
	if (bTransparent)
	{
		return ImageCache.byteCount();
	}

	QImage *backBuffer = dynamic_cast<QImage*>(backingStore()->paintDevice());
	return backBuffer->byteCount();
}

void VaQuoleWebView::getCachedEvents(QList< QPair<QString, QString> >& Events, bool bClearCache)
{
	Events = CachedScriptEvents;

	if (bClearCache)
	{
		CachedScriptEvents.clear();
	}
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
		page()->mainFrame()->addToJavaScriptWindowObject("VaQuoleWebView", this);
	}
}

void VaQuoleWebView::scriptEvent(QString event, QString message)
{
	QPair<QString, QString> NewEvent;
	NewEvent.first = event;
	NewEvent.second = message;

	CachedScriptEvents.append(NewEvent);
}

} // namespace VaQuole
