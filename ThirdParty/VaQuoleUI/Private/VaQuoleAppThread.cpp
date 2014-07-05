// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleAppThread.h"
#include "VaQuoleUILib.h"

#include <QApplication>
#include <QNetworkProxyFactory>
#include <QWebSettings>

#include <QtDebug>
#include <QFile>
#include <QThread>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QFile file(QDate::currentDate().toString("VaQuoleUI_dd_MM_yyyy.log"));
	if(!file.open(QIODevice::Append | QIODevice::Text))
	{
		return;
	}

	QTextStream out(&file);
	out << QTime::currentTime().toString("hh:mm:ss.zzz ");

	switch (type)
	{
	case QtDebugMsg:	out << "DBG"; break;
	case QtWarningMsg:  out << "WRN"; break;
	case QtCriticalMsg: out << "CRT"; break;
	case QtFatalMsg:    out << "FTL"; break;
	}

	out << " VaQuoleUI " << msg << '\n';
	out.flush();
}

namespace VaQuole
{

/** Main Qt class object */
static QApplication* pApp = NULL;

VaQuoleUIManager::~VaQuoleUIManager()
{
	qDebug() << "Trying to stop the UI thread..";

	try
	{
		stop();
		qDebug() << "Stopped.";
	}
	catch(...)
	{
		qDebug() << "Exception catched on app thread destruction";
	}
}

void VaQuoleUIManager::run()
{
	// Create
	if (QApplication::instance() == nullptr)
	{
		int argc = 1;
		char *argv[] = { "VaQuoleApp" };

#ifdef VA_DEBUG
		qInstallMessageHandler(myMessageOutput);
#endif

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
		mutex.lock();
		int PagesNum = WebPages.size();
		mutex.unlock();

		for(int i = 0; i < PagesNum; i++)
		{
			// [START] Lock pages list
			mutex.lock();

			VaQuoleWebPage* Page = WebPages.at(i);

			// [START] Lock data to read values
			Page->mutex.lock();

			UIDataKeeper* ExtComm = Page->GetData();
			Q_CHECK_PTR(ExtComm);

			VaQuoleWebView* WebView = WebViews.value(ExtComm->ObjectId, NULL);
			if(WebView == NULL)
			{
				WebView = new VaQuoleWebView();
				WebView->show();
				WebViews.insert(ExtComm->ObjectId, WebView);
			}

			QString NewURL = ExtComm->NewURL;

			bool bNewTransparency = ExtComm->bDesiredTransparency;
			int NewWidth = ExtComm->DesiredWidth;
			int NewHeight = ExtComm->DesiredHeight;

			// Check primary visuals update is necessary
			bool bTransparencyChanged = bNewTransparency != WebView->getTransparency();
			bool bSizeChanged = (WebView->width() != NewWidth || WebView->height() != NewHeight);

			// Extract mouse events
			QList<MouseEvent> MouseEvents = ExtComm->MouseEvents;

			// External data update
			ExtComm->NewURL = "";
			ExtComm->bTransparent = WebView->getTransparency();
			ExtComm->Width = WebView->width();
			ExtComm->Height = WebView->height();
			ExtComm->MouseEvents.clear();

			// Update grabbed view
			ExtComm->ImageBits = WebView->getImageData();

			// [END] Unlock page data
			Page->mutex.unlock();

			// [END] Unlock pages list
			mutex.unlock();

			// Check primary visual changes
			if(bTransparencyChanged || bSizeChanged)
			{
				if( bTransparencyChanged )
				{
					WebView->setTransparent(bNewTransparency);
				}

				if( bSizeChanged )
				{
					WebView->resize(NewWidth, NewHeight);
				}

				// Process events immediately
				qApp->processEvents();

				// Update external data
				Page->mutex.lock();
				ExtComm->bTransparent = WebView->getTransparency();
				ExtComm->Width = WebView->width();
				ExtComm->Height = WebView->height();
				ExtComm->ImageBits = WebView->getImageData();
				Page->mutex.unlock();
			}

			// Check URL
			if(!NewURL.isEmpty())
			{
				qDebug() << "Load url:" << NewURL;
				WebView->load(QUrl(NewURL));
			}

			// Process mouse events
			MouseEvent Event;
			foreach(Event, MouseEvents)
			{
				if(Event.button == Qt::NoButton)
				{
					// It's a mouse move event
					VaQuole::simulateMouseMove(WebView, Event.eventPos);
				}
				else
				{
					VaQuole::simulateMouseClick(WebView, Event.eventPos, Event.button, Event.modifiers, Event.bButtonPressed);
				}
			}

			// Update pages num to be sure that we're in array bounds
			mutex.lock();
			PagesNum = WebPages.size();
			mutex.unlock();
		}

		// Process Qt events
		qApp->processEvents();

		// Clean pages marked for delete
		mutex.lock();
		PagesNum = WebPages.size();
		for(int j = 0; j < WebPages.size(); )
		{
			if(WebPages.at(j)->GetData()->bMarkedForDelete)
			{
				VaQuoleWebView* ViewToDelete = WebViews.value(WebPages.at(j)->GetData()->ObjectId);
				VaQuoleWebPage* PageToDelete = WebPages.at(j);

				WebViews.remove(WebPages.at(j)->GetData()->ObjectId);
				WebPages.removeAt(j);

				delete ViewToDelete;
				delete PageToDelete->GetData();
				delete PageToDelete;
			}
			else
			{
				j++;
			}
		}
		mutex.unlock();

		// Check we've closed some windows
		if( PagesNum != WebPages.size() )
		{
			qDebug() << "Process pages deletion:" << WebPages.size() << PagesNum;
			qApp->processEvents();
		}
	}

	qDebug() << "About to exit";
}

void VaQuoleUIManager::AddPage(VaQuoleWebPage *Page)
{
	std::lock_guard<std::mutex> guard(mutex);

	WebPages.append(Page);
}

void VaQuoleUIManager::UpdateImageBuffer(UIDataKeeper *ExtComm, int DataSize)
{
	if(ExtComm->ImageDataSize != DataSize)
	{
		if(ExtComm->ImageBits)
		{
			delete ExtComm->ImageBits;
		}

		ExtComm->ImageBits = new uchar[DataSize];
	}
}

} // namespace VaQuole
