// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleInputHelpers.h"

#include <QApplication>
#include <QPoint>
#include <QWidget>

#include <QMouseEvent>
#include <QContextMenuEvent>

namespace VaQuole
{

//////////////////////////////////////////////////////////////////////////
// Mouse input

QMouseEvent* createMouseEvent(	const QWidget* const pWidget,
								const QEvent::Type eventType,
								const QPoint& widgetPos,
								const Qt::MouseButton buttons,
								const Qt::KeyboardModifiers modifiers,
								Qt::MouseButton buttonCausingEvent)
{
	if (pWidget == NULL)
	{
		return NULL;
	}

	QMouseEvent* pEvent = new QMouseEvent(	eventType,
											widgetPos,
											pWidget->mapToGlobal(widgetPos),
											buttonCausingEvent,
											buttons,
											modifiers);

	return pEvent;
}

void simulateMouseClick(QWidget* const pWidget,
						const QPoint& widgetPos,
						const Qt::MouseButton button,
						const Qt::KeyboardModifiers modifiers,
						const bool bButtonPressed)
{
	if (pWidget == NULL || QApplication::instance() == NULL)
	{
		return;
	}

	QMouseEvent* pMouseEvent;

	if(bButtonPressed)
	{
		pMouseEvent = createMouseEvent(	pWidget, QEvent::MouseButtonPress,
										widgetPos, button,
										modifiers, button);
	}
	else
	{
		pMouseEvent = createMouseEvent(	pWidget, QEvent::MouseButtonRelease,
										widgetPos, button,
										modifiers, button);
	}

	QApplication::instance()->postEvent(pWidget, pMouseEvent);
}

void simulateMouseMove(	QWidget* const pWidget, const QPoint& widgetPos)
{
	if (pWidget == NULL || QApplication::instance() == NULL)
	{
		return;
	}

	QMouseEvent* pMouseEvent = createMouseEvent(pWidget, QEvent::MouseMove,
										widgetPos, Qt::NoButton,
										Qt::NoModifier, Qt::NoButton);

	QApplication::instance()->postEvent(pWidget, pMouseEvent);
}

void simulateMouseWheel(QWidget* const pWidget,
						const QPoint &widgetPos,
						const Qt::KeyboardModifiers modifiers,
						const bool bWheelDown)
{
	if (pWidget == NULL || QApplication::instance() == NULL)
	{
		return;
	}

	/** Most mouse types work in steps of 15 degrees, in which case
	 * the delta value is a multiple of 120; i.e., 120 units * 1/8 = 15 degrees. */
	int delta = 120;
	if( bWheelDown )
	{
		delta *= -1;
	}

	QWheelEvent* pEvent = new QWheelEvent(	widgetPos,
											pWidget->mapToGlobal(widgetPos),
											delta,
											Qt::NoButton,
											modifiers);

	QApplication::instance()->postEvent(pWidget, pEvent);
}

void simulateContextMenu(	QWidget* const pWidget,
							const QPoint& widgetPos,
							const Qt::KeyboardModifiers modifiers)
{
	QContextMenuEvent* pEvent = new QContextMenuEvent(QContextMenuEvent::Mouse,
													 widgetPos,
													 pWidget->mapToGlobal(widgetPos),
													 modifiers);

	QApplication::instance()->postEvent(pWidget, pEvent);
}


//////////////////////////////////////////////////////////////////////////
// Keyboard input

QKeyEvent* createKeyEvent(	const QEvent::Type eventType,
							const int key,
							const Qt::KeyboardModifiers modifiers,
							QString & text,
							bool autorep,
							ushort count)
{
	QKeyEvent* pEvent = new QKeyEvent(	eventType,
										key,
										modifiers,
										text,
										autorep,
										count);

	return pEvent;
}

void simulateKey(	QWidget* const pWidget,
					const unsigned int key,
					const Qt::KeyboardModifiers modifiers,
					QString & text,
					const bool bKeyPressed)
{
	if (pWidget == NULL || QApplication::instance() == NULL)
	{
		return;
	}

	QKeyEvent* pKeyEvent;

	if(bKeyPressed)
	{
		pKeyEvent = createKeyEvent(QEvent::KeyPress, key, modifiers, text);
	}
	else
	{
		pKeyEvent = createKeyEvent(QEvent::KeyRelease, key, modifiers, text);
	}

	QApplication::instance()->sendEvent(pWidget, pKeyEvent);
}

} // namespace VaQuole
