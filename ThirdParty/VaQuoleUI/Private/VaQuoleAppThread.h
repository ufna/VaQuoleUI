// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEAPPTHREAD_H
#define VAQUOLEAPPTHREAD_H

#include "VaQuolePublicPCH.h"
#include "VaQuoleWebView.h"
#include "VaQuoleInputHelpers.h"

#include <atomic>
#include <mutex>
#include <thread>

#include <QHash>
#include <QList>
#include <QString>
#include <QUuid>

namespace VaQuole
{

class VaQuoleWebUI;

/**
 * Keeps data that we can't store in header because of non-Qt lib use
 */
struct UIDataKeeper
{
	/** Unique object instance id */
	QString ObjectId;

	/** Is page is enabled in game? */
	bool bEnabled;

	/** Is page should be deleted? */
	bool bMarkedForDelete;

	/** Is last desired page loaded or nor? */
	bool bPageLoaded;

	/** URL managament */
	QString NewURL;

	/** Transparency */
	bool bTransparent;
	bool bDesiredTransparency;

	/** Window size */
	int Width;
	int Height;
	int DesiredWidth;
	int DesiredHeight;

	/** Image data */
	uchar* ImageBits;
	int ImageDataSize;

	/** Input data */
	QList<MouseEvent> MouseEvents;
	QList<KeyEvent> KeyEvents;

	/** JavaScript data stored in QList to keep strict order */
	QList< QPair<QString, QString> > ScriptCommands;	// Uuid, ScriptSource
	QList< QPair<QString, QString> > ScriptResults;		// Uuid, ReturnValue
	QList< QPair<QString, QString> > ScriptEvents;		// Event, Message

	/** Defaults */
	UIDataKeeper::UIDataKeeper()
		: ObjectId(QUuid::createUuid().toString())
	{
		bEnabled = false;
		bMarkedForDelete = false;
		bPageLoaded = false;

		bDesiredTransparency = false;
		DesiredWidth = 32;
		DesiredHeight = 32;

		ImageBits = NULL;
		ImageDataSize = 0;
	}
};

/**
 * Runnable thread manager
 */
class VaThread
{
public:
	VaThread() : m_stop(false), m_thread() { }
	virtual ~VaThread() { try { stop(); } catch(...) { /* make something */ } }

	VaThread(VaThread const&) = delete;
	VaThread& operator =(VaThread const&) = delete;

	void stop() { m_stop = true; m_thread.join(); }
	void start() { m_thread = std::thread(&VaThread::run, this); }

protected:
	virtual void run() = 0;
	std::atomic<bool> m_stop;

private:
	std::thread m_thread;
};

/**
 * Class that manages all UI views
 */
class VaQuoleUIManager : public VaThread
{
	// Begin VaThread Interface
public:
	~VaQuoleUIManager();

protected:
	void run();
	// End VaThread Interface

public:
	void AddPage(VaQuoleWebUI *Page);

private:
	/** Check that we have valid buffer to keep the image bits */
	void UpdateImageBuffer(UIDataKeeper *ExtComm, VaQuoleWebView *WebView);

private:
	/** Locker to be used with external commands */
	std::mutex mutex;

	/** List of all opened web pages */
	QList<VaQuoleWebUI*> WebPages;

	/** Map of all Qt WebView windows */
	QHash<QString, VaQuoleWebView*> WebViews;

};

} // namespace VaQuole

#endif // VAQUOLEAPPTHREAD_H
