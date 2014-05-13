// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEWEBVIEW_H
#define VAQUOLEWEBVIEW_H

#include <QWebView>

class QImage;

class VaQuoleWebView : public QWebView
{
	Q_OBJECT

public:
	explicit VaQuoleWebView(QWidget *parent = 0);

	/** Changes background transparency */
	void setTransparent(bool transparent = true);

	/** Resizes the View */
	void resize(int w, int h);

	/** Get refence to image cache data */
	const uchar * getImageData();

private:
	/** Recreates image cache buffer */
	void updateImageCache(QSize ImageSize = QSize());

	/** Rendered widget buffer */
	QImage ImageCache;

	/** Indicates whether the View is transparent or composed on white */
	bool bTransparent;

protected:
	void paintEvent(QPaintEvent*);

};

#endif // VAQUOLEWEBVIEW_H
