#-------------------------------------------------
#
# Project created by QtCreator 2014-05-11T04:22:46
#
#-------------------------------------------------

QT       += network webkit webkitwidgets

TARGET = VaQuoleUILib
TEMPLATE = lib
CONFIG += staticlib

DEFINES += _UNICODE NOT_UE

win32 {
    !contains(QMAKE_TARGET.arch, x86_64) {
	message("Building 32bit version")
	DESTDIR = ../VaQuoleUI/Lib/Win32

    } else {
	message("Building 64bit version")
	DESTDIR = ../VaQuoleUI/Lib/Win64
    }
}

INCLUDEPATH += ./Include \
    ./Private

SOURCES += Private/VaQuoleUILib.cpp \
    Private/VaQuoleWebView.cpp \
    Private/VaQuoleInputHelpers.cpp \
    Private/VaQuoleAppThread.cpp

HEADERS += Include/VaQuoleUILib.h \
    Private/VaQuoleWebView.h \
    Include/VaQuolePublicPCH.h \
    Private/VaQuoleInputHelpers.h \
    Private/VaQuoleAppThread.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
