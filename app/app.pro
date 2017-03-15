QT      += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    Parser.cpp \
    MainWindow.cpp \
    Function.cpp

HEADERS += \
    Function.h \
    Parser.h \
    Token.h \
    MainWindow.h

FORMS   += MainWindow.ui

CONFIG  += c++14
