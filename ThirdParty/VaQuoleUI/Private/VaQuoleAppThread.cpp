// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleAppThread.h"
#include "VaQuoleUILib.h"

#include <QApplication>
#include <QNetworkProxyFactory>
#include <QWebSettings>

namespace VaQuole
{

/** Main Qt class object */
static QApplication* pApp = NULL;

void VaQuoleUIManager::run()
{
	// Create
	if (QApplication::instance() == nullptr)
	{
		int argc = 1;
		char *argv[] = { "VaQuoleApp" };

		pApp = new QApplication(argc, argv);
		pApp->setQuitOnLastWindowClosed(false);
		pApp->processEvents();

		// Set network config
		QNetworkProxyFactory::setUseSystemConfiguration (true);
		QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
		QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);
	}

	while (!m_stop)
	{
		int PagesNum = WebPages.size();
		for(int i = 0; i < PagesNum; i++)
		{
			// Get key pointers set
			mutex.lock();
			VaQuoleWebPage* Page = WebPages.at(i);
			UIDataKeeper* ExtComm = Page->GetData();
			Q_CHECK_PTR(ExtComm);
			VaQuoleWebView* WebView = WebViews.value(ExtComm->ObjectId, NULL);
			if(WebView == NULL)
			{
				WebView = new VaQuoleWebView();
				WebView->show();
				WebViews.insert(ExtComm->ObjectId, WebView);
			}
			mutex.unlock();

			// Check URL
			mutex.lock();
			QString NewURL = ExtComm->NewURL;
			ExtComm->NewURL = "";
			mutex.unlock();

			// Check transparency change
			mutex.lock();
			bool bTransparencyChanged = ExtComm->bTransparencyChanged;
			bool bTransparent = ExtComm->bTransparent;
			mutex.unlock();

			// Apply commands
			if(!NewURL.isEmpty())
			{
				WebView->load(QUrl(NewURL));
			}

			if(bTransparencyChanged)
			{
				WebView->setTransparent(bTransparent);
			}
		}

		// Process Qt events
		qApp->processEvents();
	}
}

void VaQuoleUIManager::AddPage(VaQuoleWebPage *Page)
{
	std::lock_guard<std::mutex> guard(mutex);

	WebPages.append(Page);
}

} // namespace VaQuole
