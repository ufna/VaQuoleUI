// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEINPUTHELPERS_H
#define VAQUOLEINPUTHELPERS_H

#include <QEvent>

class QMouseEvent;
class QWidget;
class QPoint;

namespace VaQuole
{

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


} // namespace VaQuole

#endif // VAQUOLEINPUTHELPERS_H
