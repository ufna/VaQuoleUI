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
	~VaQuoleWebView();

	void resize(int w, int h);

	QImage ImageCache;

private:
	void updateImageCache();

protected:
	void resizeEvent(QResizeEvent*);
	void paintEvent(QPaintEvent*);

};

#endif // VAQUOLEWEBVIEW_H
