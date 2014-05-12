// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleWebView.h"

#include <QDebug>
#include <QWebFrame>
#include <QPaintEvent>
#include <QTimer>

VaQuoleWebView::VaQuoleWebView(QWidget *parent) :
	QWebView(parent)
{
	ImageCache = QImage(256, 256, QImage::Format_ARGB32);
}

VaQuoleWebView::~VaQuoleWebView()
{

}

void VaQuoleWebView::updateImageCache()
{
	ImageCache = QImage(size(), QImage::Format_ARGB32);
}

void VaQuoleWebView::resize(int w, int h)
{
	QWebView::resize(w, h);

	updateImageCache();
}

void VaQuoleWebView::resizeEvent(QResizeEvent *e)
{
	QWebView::resizeEvent(e);

	updateImageCache();
}

void VaQuoleWebView::paintEvent(QPaintEvent *ev)
{
	QWebPage *page = this->page();

	if (!page)
		return;

	QWebFrame *frame = page->mainFrame();
	QPainter p;
	p.begin(&ImageCache);
	p.setRenderHints(renderHints());

	frame->render(&p, ev->region());
	p.end();

	p.begin(this);
	p.setRenderHints(renderHints());
	p.drawImage(0,0,ImageCache);
	p.end();

	ev->setAccepted(true);
}
