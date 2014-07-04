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

	/** Count cached commands */
	int getCachedCommandsNumber();

	/** Get one particular command */
	QString getCachedCommand(int index);

	/** Clears cached JS commands */
	void clearCachedCommands();


private:
	/** Recreates image cache buffer */
	void updateImageCache(QSize ImageSize = QSize());

	/** Rendered widget buffer */
	QImage ImageCache;

	/** Indicates whether the View is transparent or composed on white */
	bool bTransparent;

	/** Cache of commands received from JS */
	QStringList cachedJavaScriptCommands;


protected:
	void paintEvent(QPaintEvent*);

private slots:
	/** Puts reference to this class object into JS code */
	void registerJavaScriptWindowObject(bool pageLoaded);

public slots:
	/** Simple text command from javascript received here */
	void scriptCommand(QString command);

};

} // namespace VaQuole

#endif // VAQUOLEWEBVIEW_H
