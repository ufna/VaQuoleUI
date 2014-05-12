// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEUILIB_H
#define VAQUOLEUILIB_H

/** Some defines missed in QtCreator */
typedef unsigned char uchar;

#ifdef UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif

/** Forward declatation of Qt class members */
class QImage;
class VaQuoleWebView;

namespace VaQuole
{

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
class VaQuoleUI
{

public:
	VaQuoleUI();

	/** Destroy web view and clear memory */
	void Destroy();

	/** Open URL in the View */
	void OpenURL(const TCHAR *NewURL);

	/** Load page with HTML5 benchmark */
	void OpenBenchmark();

	/** Get reference to grabbed screen texture */
	const uchar * GrabViewC();
	uchar * GrabView();

	/** Clear grabbed view memory */
	void ClearView();

	/** Set desired few size */
	void Resize(int w, int h);

private:
	VaQuoleWebView *View;
	QImage *CachedImg;

};


} // namespace VaQuole

#endif // VAQUOLEUILIB_H
