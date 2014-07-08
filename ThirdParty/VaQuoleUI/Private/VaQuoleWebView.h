// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEWEBVIEW_H
#define VAQUOLEWEBVIEW_H

#include "VaQuolePublicPCH.h"

#include <QWebView>

class QImage;

namespace VaQuole
{

class VaQuoleWebView : public QWebView
{
	Q_OBJECT

public:
	explicit VaQuoleWebView(QWidget *parent = 0);

	/** Get current background transparency state */
	bool getTransparency() const;

	/** Changes background transparency */
	void setTransparent(bool transparent = true);

	/** Resizes the View */
	void resize(int w, int h);

	/** Get refence to image cache data */
	uchar * getImageData();

	/** Cached image data size to make a memcopy */
	int getImageDataSize();

	/** Get cached events from JavaScript and optionally clear cache */
	void getCachedEvents(QList< QPair<QString, QString> >& Events, bool bClearCache = true);


private:
	/** Recreates image cache buffer */
	void updateImageCache(QSize ImageSize = QSize());

	/** Rendered widget buffer */
	QImage ImageCache;

	/** Indicates whether the View is transparent or composed on white */
	bool bTransparent;

	/** Events received from JavaScript */
	QList< QPair<QString, QString> > CachedScriptEvents;		// Event, Message


protected:
	void paintEvent(QPaintEvent*);

private slots:
	/** Puts reference to this class object into JS code */
	void registerJavaScriptWindowObject(bool pageLoaded);

public slots:
	/** Callback from JavaSript */
	void scriptEvent(QString event, QString message = QString());

};

} // namespace VaQuole

#endif // VAQUOLEWEBVIEW_H
