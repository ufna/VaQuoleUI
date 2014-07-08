// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEINPUTHELPERS_H
#define VAQUOLEINPUTHELPERS_H

#include "VaQuolePublicPCH.h"

#include <QEvent>
#include <QString>
#include <QPoint>

class QMouseEvent;
class QKeyEvent;
class QWidget;

namespace VaQuole
{

//////////////////////////////////////////////////////////////////////////
// Data structs

struct MouseEvent
{
	QPoint eventPos;
	Qt::MouseButton button;
	Qt::KeyboardModifiers modifiers;
	bool bButtonPressed;

	/** Scroll event helpers */
	bool bScrollUp;
	bool bScrollDown;

	MouseEvent::MouseEvent()
	{
		button = Qt::NoButton;
		modifiers = Qt::NoModifier;
		bButtonPressed = false;

		bScrollUp = false;
		bScrollDown = false;
	}
};

struct KeyEvent
{
	Qt::Key key;
	Qt::KeyboardModifiers modifiers;
	bool bKeyPressed;
	QString text;

	KeyEvent::KeyEvent()
	{
		key = Qt::Key_unknown;
		modifiers = Qt::NoModifier;
		bKeyPressed = false;
		text = QString();
	}
};


//////////////////////////////////////////////////////////////////////////
// Mouse input

QMouseEvent* createMouseEvent(	const QWidget* const pWidget,
								const QEvent::Type eventType,
								const QPoint& widgetPos,
								const Qt::MouseButton buttons,
								const Qt::KeyboardModifiers modifiers,
								Qt::MouseButton buttonCausingEvent);

void simulateMouseClick(	QWidget* const pWidget,
							const QPoint& widgetPos,
							const Qt::MouseButton button,
							const Qt::KeyboardModifiers modifiers,
							const bool bButtonPressed);

void simulateMouseMove(QWidget* const pWidget, const QPoint& widgetPos);

/** Default wheel speed is used. Additional buttons are not supported */
void simulateMouseWheel(	QWidget* const pWidget,
							const QPoint& widgetPos,
							const Qt::KeyboardModifiers modifiers,
							const bool bWheelDown = true);

/** Right mouse click for QWebView */
void simulateContextMenu(	QWidget* const pWidget,
							const QPoint& widgetPos,
							const Qt::KeyboardModifiers modifiers);


//////////////////////////////////////////////////////////////////////////
// Keyboard input

QKeyEvent* createKeyEvent(	const QEvent::Type eventType,
							const int key,
							const Qt::KeyboardModifiers modifiers,
							QString & text = QString(),
							bool autorep = false,
							ushort count = 1);

/** Attn.! Obsolete - QtTest used instead */
void simulateKey(	QWidget* const pWidget,
					const unsigned int key,
					const Qt::KeyboardModifiers modifiers,
					QString & text = QString(),
					const bool bKeyPressed = true);


} // namespace VaQuole

#endif // VAQUOLEINPUTHELPERS_H
