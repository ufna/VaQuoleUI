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

	void MouseMove(int x, int y);
	void MouseClick(int x, int y, VaQuole::EMouseButton::Type button,
					bool bPressed = true,
					unsigned int modifiers = VaQuole::EKeyboardModifier::NoModifier);

	void InputKey(	const unsigned int key,
					const bool bPressed = true,
					const unsigned int modifiers = VaQuole::EKeyboardModifier::NoModifier);


	//////////////////////////////////////////////////////////////////////////
	// Internal data

private:
	/** Locker to be used with external commands */
	std::mutex mutex;

	/** Data keeper to store external commands */
	UIDataKeeper* ExtComm;

};


} // namespace VaQuole

#endif // VAQUOLEUILIB_H
