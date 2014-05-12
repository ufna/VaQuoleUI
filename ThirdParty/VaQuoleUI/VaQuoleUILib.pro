#-------------------------------------------------
#
# Project created by QtCreator 2014-05-11T04:22:46
#
#-------------------------------------------------

QT       += network webkit webkitwidgets

TARGET = VaQuoleUILib
TEMPLATE = lib
CONFIG += staticlib

DEFINES += _UNICODE

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
    Private/VaQuoleWebView.cpp

HEADERS += Include/VaQuoleUILib.h \
    Private/VaQuoleWebView.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
