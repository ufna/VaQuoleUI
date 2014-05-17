// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEINPUTHELPERS_H
#define VAQUOLEINPUTHELPERS_H

#include <QEvent>
#include <QString>

class QMouseEvent;
class QKeyEvent;
class QWidget;
class QPoint;

namespace VaQuole
{

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
