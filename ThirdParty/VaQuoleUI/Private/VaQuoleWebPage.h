// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEWEBPAGE_H
#define VAQUOLEWEBPAGE_H

#include "VaQuolePublicPCH.h"

#include <QWebPage>
#include <QString>

namespace VaQuole
{

class VaQuoleWebPage : public QWebPage
{
	Q_OBJECT

public:
	explicit VaQuoleWebPage(QObject* parent = 0);

protected:
	QString	chooseFile(QWebFrame * parentFrame, const QString & suggestedFile);
	void javaScriptAlert(QWebFrame * frame, const QString & msg);
	bool javaScriptConfirm(QWebFrame * frame, const QString & msg);
	void javaScriptConsoleMessage(const QString & message, int lineNumber, const QString & sourceID);
	bool javaScriptPrompt(QWebFrame * frame, const QString & msg, const QString & defaultValue, QString * result);

};

} // namespace VaQuole

#endif // VAQUOLEWEBPAGE_H
