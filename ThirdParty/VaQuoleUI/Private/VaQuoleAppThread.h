// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEAPPTHREAD_H
#define VAQUOLEAPPTHREAD_H

#include "VaQuolePublicPCH.h"

#include <QThread>

class QApplication;
class QWebView;
class VaQuoleWebView;

class VaQuoleAppThread : public QThread
{
	Q_OBJECT

public:
	explicit VaQuoleAppThread(QObject *parent = 0);
	~VaQuoleAppThread();

	void stopThread();

	void createView(int w = 256, int h = 256);
	void closeView();

	void openURL(const QString& NewURL);
	void evaluateJavaScript(const QString & scriptSource);

	void setTransparent(bool transparent = true);
	void resize(int w, int h);

	const uchar * grabView();


	//////////////////////////////////////////////////////////////////////////
	// Player input

	void mouseMove(int x, int y);
	void mouseClick(int x, int y, VaQuole::EMouseButton::Type button,
					bool bPressed = true,
					unsigned int modifiers = VaQuole::EKeyboardModifier::NoModifier);

	void inputKey(const unsigned int key,
				  const bool bPressed = true,
				  const unsigned int modifiers = VaQuole::EKeyboardModifier::NoModifier);


protected:
	void run();

private:
	/** Main Qt class object */
	QApplication* pApp;

	/** Web page renderer */
	VaQuoleWebView *View;

};

#endif // VAQUOLEAPPTHREAD_H
