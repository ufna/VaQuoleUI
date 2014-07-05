// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEUILIB_H
#define VAQUOLEUILIB_H

#include "VaQuolePublicPCH.h"

#include <mutex>

namespace VaQuole
{

class VaQuoleWebPage;
struct UIDataKeeper;

/**
 * Common library functions
 */
extern "C"
{
	/** Initialize QApplication */
	void Init();

	/** Clean all Qt and lib related data */
	void Cleanup();

	/** Construct new web page view */
	VaQuoleWebPage* ConstructNewPage();
}

/**
 * Class that handles view of one web page
 */
class VaQuoleWebPage
{
public:
#ifdef NOT_UE
	/** Create new instance of web UI */
	VaQuoleWebPage();

	/** Register UI page in global UI manager */
	void Register();

	/** Get internal data pointer */
	UIDataKeeper* GetData();
#endif

	/** Destroy web view and clear memory */
	void Destroy();

	/** Open URL in the View */
	void OpenURL(const TCHAR *NewURL);

	/** Load page with HTML5 benchmark */
	void OpenBenchmark();

	/** Evaluate JS script on current page */
	void EvaluateJavaScript(const TCHAR *ScriptSource);

	/** Get reference to grabbed screen texture */
	const uchar * GrabView();

	/** Change background transparency */
	void SetTransparent(bool Transparent = true);

	/** Set desired few size */
	void Resize(int w, int h);

	/** Is resize and transparency commands are processes? */
	bool IsPendingVisualEvents();


	//////////////////////////////////////////////////////////////////////////
	// JS commands callback

	/** Count cached commands */
	int GetCachedCommandsNumber();

	/** Get one particular command */
	TCHAR * GetCachedCommand(int Index);

	/** Clear commands cache */
	void ClearCachedCommands();


	//////////////////////////////////////////////////////////////////////////
	// Player input

	void InputMouse(int X, int Y, VaQuole::EMouseButton::Type Button = VaQuole::EMouseButton::NoButton,
					bool bMouseDown = true,
					const VaQuole::KeyModifiers Modifiers = VaQuole::KeyModifiers());

	void InputKey(	const TCHAR *Key,
					const bool bPressed = true,
					const VaQuole::KeyModifiers Modifiers = VaQuole::KeyModifiers());


	//////////////////////////////////////////////////////////////////////////
	// Internal data
public:
	/** Locker to be used with external commands */
	std::mutex mutex;

private:
	/** Data keeper to share data between threads */
	UIDataKeeper* ExtComm;

};

} // namespace VaQuole

#endif // VAQUOLEUILIB_H
