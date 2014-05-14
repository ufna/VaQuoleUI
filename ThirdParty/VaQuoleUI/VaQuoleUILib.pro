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

contains(CONFIG,Win64){
    message("Building 64bit version")
    DESTDIR = ../VaQuoleUI/Lib/Win64
}else{
    message("Building 32bit version")
    DESTDIR = ../VaQuoleUI/Lib/Win32
}

INCLUDEPATH += ./Include \
    ./Private

SOURCES += Private/VaQuoleUILib.cpp \
    Private/VaQuoleWebView.cpp \
    Private/VaQuoleAppThread.cpp \
    Private/VaQuoleInputHelpers.cpp

HEADERS += Include/VaQuoleUILib.h \
    Private/VaQuoleWebView.h \
    Private/VaQuoleAppThread.h \
    Include/VaQuolePublicPCH.h \
    Private/VaQuoleInputHelpers.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
