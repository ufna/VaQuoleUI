// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEUILIB_H
#define VAQUOLEUILIB_H

/** Forward declatation of Qt class members */
class QWebView;

namespace VaQuole
{

typedef unsigned char uchar;


/**
 * Common library functions
 */
extern "C"
{
	/** Initialize QApplication */
	void Init();

	/** Process Qt events (we have to use it because lib lives in main thread */
	void Update();

	/** Clean all Qt and lib related data */
	void Cleanup();
}


/**
 * Class that handles view of one web page
 */
class VaQuoleWebView
{

public:
	VaQuoleWebView();
};


} // namespace VaQuole

#endif // VAQUOLEUILIB_H
