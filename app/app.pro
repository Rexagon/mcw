TEMPLATE = app

TARGET	= mcw
QT      += core gui widgets
CONFIG  += c++14

DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR	= ../../build
OBJECTS_DIR = obj
MOC_DIR = obj

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
