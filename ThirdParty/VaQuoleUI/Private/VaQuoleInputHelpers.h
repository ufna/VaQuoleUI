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

	MouseEvent::MouseEvent()
	{
		button = Qt::NoButton;
		modifiers = Qt::NoModifier;
		bButtonPressed = false;
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

void simulateMouseMove(	QWidget* const pWidget, const QPoint& widgetPos);


//////////////////////////////////////////////////////////////////////////
// Keyboard input

QKeyEvent* createKeyEvent(	const QEvent::Type eventType,
							int key,
							const Qt::KeyboardModifiers modifiers,
							QString & text = QString(),
							bool autorep = false,
							ushort count = 1);

void simulateKey(	QWidget* const pWidget,
					const unsigned int key,
					const Qt::KeyboardModifiers modifiers,
					const bool bKeyPressed);


} // namespace VaQuole

#endif // VAQUOLEINPUTHELPERS_H
