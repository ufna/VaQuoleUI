// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleWebPage.h"

#include <QDebug>

namespace VaQuole
{

VaQuoleWebPage::VaQuoleWebPage(QObject *parent) :
	QWebPage(parent)
{

}

QString	VaQuoleWebPage::chooseFile(QWebFrame * parentFrame, const QString & suggestedFile)
{
	qDebug() << "chooseFile:" << suggestedFile;

	return suggestedFile;
}

void VaQuoleWebPage::javaScriptAlert(QWebFrame * frame, const QString & msg)
{
	qDebug() << "JsAlert:" << msg;
}

bool VaQuoleWebPage::javaScriptConfirm(QWebFrame * frame, const QString & msg)
{
	return false;
}

void VaQuoleWebPage::javaScriptConsoleMessage(const QString & message, int lineNumber, const QString & sourceID)
{
	qDebug() << "JsConsole [" << lineNumber << "] [" << sourceID << "]:" << message;
}

bool VaQuoleWebPage::javaScriptPrompt(QWebFrame * frame, const QString & msg, const QString & defaultValue, QString * result)
{
	return false;
}

} // namespace VaQuole
