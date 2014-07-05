// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEAPPTHREAD_H
#define VAQUOLEAPPTHREAD_H

#include "VaQuolePublicPCH.h"
#include "VaQuoleWebView.h"

#include <atomic>
#include <mutex>
#include <thread>

#include <QHash>
#include <QList>
#include <QString>
#include <QUuid>

namespace VaQuole
{

class VaQuoleWebPage;

/**
 * Keeps data that we can't store in header because of non-Qt lib use
 */
struct UIDataKeeper
{
	/** Unique object instance id */
	QString ObjectId;

	/** Is page should be deleted? */
	bool bMarkedForDelete;

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

	/** Defaults */
	UIDataKeeper::UIDataKeeper()
		: ObjectId(QUuid::createUuid().toString())
	{
		bMarkedForDelete = false;

		bDesiredTransparency = false;
		DesiredWidth = 32;
		DesiredHeight = 32;

		ImageBits = 0;
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
	void AddPage(VaQuoleWebPage *Page);

private:
	/** Check that we have valid buffer to keep the image bits */
	void UpdateImageBuffer(UIDataKeeper *ExtComm, int DataSize);

private:
	/** Locker to be used with external commands */
	std::mutex mutex;

	/** List of all opened web pages */
	QList<VaQuoleWebPage*> WebPages;

	/** Map of all Qt WebView windows */
	QHash<QString, VaQuoleWebView*> WebViews;

};

} // namespace VaQuole

#endif // VAQUOLEAPPTHREAD_H
