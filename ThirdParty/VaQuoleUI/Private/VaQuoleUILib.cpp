// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUILib.h"

#include <QApplication>

namespace VaQuole
{

/** Main Qt class object */
static QApplication* pApp = NULL;


//////////////////////////////////////////////////////////////////////////
// Common lib functions

void Init()
{
	if (QApplication::instance() == nullptr)
	{
		int argc = 1;
		char arg1[] = "VaQuoleUILib";
		char* argv[1] = {arg1};
		pApp = new QApplication(argc, argv);
		pApp->setQuitOnLastWindowClosed(false);
	}
}

void Update()
{
	if(pApp)
	{
		pApp->processEvents();
	}
}

void Cleanup()
{
	if(pApp)
	{
		pApp->quit();
		delete pApp;
		pApp = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
// WebView class

VaQuoleWebView::VaQuoleWebView()
{
}

} // namespace VaQuole
