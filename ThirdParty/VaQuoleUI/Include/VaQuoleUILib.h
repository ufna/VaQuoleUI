// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEUILIB_H
#define VAQUOLEUILIB_H

#include "VaQuolePublicPCH.h"

#include <mutex>
#include <vector>

namespace VaQuole
{

class VaQuoleWebUI;
struct UIDataKeeper;

/**
 * Common library functions
 */
extern "C"
{
	/** Initialize QApplication */
	void Init();

	/** Initialize UE4->Qt key map */
	void InitKeyMaps();

	/** Clean all Qt and lib related data */
	void Cleanup();

	/** Construct new web page view */
	VaQuoleWebUI* ConstructNewUI();
}

/**
 * Class that handles view of one web page
 */
class VaQuoleWebUI
{
public:
#ifdef NOT_UE
	/** Create new instance of web UI */
	VaQuoleWebUI();

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
	TCHAR* EvaluateJavaScript(const TCHAR *ScriptSource);

	/** Get reference to grabbed screen texture */
	const uchar * GrabView();

	/** Is the view grabbed bits update enabled? */
	bool IsEnabled();

	/** Disabled view doesn't update grabbed bits because of performance considerations */
	void SetEnabled(bool Enabled = true);

	/** Is background marked as transparent? */
	bool IsTransparent();

	/** Change background transparency */
	void SetTransparent(bool Transparent = true);

	/** Is desired page loaded or nor? */
	bool IsPageLoaded();

	/** Set desired few size */
	void Resize(int w, int h);

	/** Is resize and transparency commands are processes? */
	bool IsPendingVisualEvents();


	//////////////////////////////////////////////////////////////////////////
	// JavaScript commands callback

	/** Get current evaluated scripts return values */
	void GetScriptResults(std::vector<ScriptEval>& Evals);

	/** Get events triggered by scripts */
	void GetScriptEvents(std::vector<ScriptEvent> &Events);


	//////////////////////////////////////////////////////////////////////////
	// Player input

	void InputMouse(int X, int Y, VaQuole::EMouseButton::Type Button = VaQuole::EMouseButton::NoButton,
					bool bMouseDown = true,
					const VaQuole::KeyModifiers Modifiers = VaQuole::KeyModifiers());

	void InputKey(	const TCHAR *Key,
					const unsigned int KeyCode,
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
