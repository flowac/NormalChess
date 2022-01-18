TARGET		= Chess
DESTDIR		= bin
MOC_DIR		= bin
OBJECTS_DIR	= bin
QT		+= core network widgets

#DEFINES += QT_DEPRECATED_WARNINGS
HEADERS += lib.h
SOURCES += app.cpp chess.cpp log.cpp web.cpp main.cpp

QMAKE_CXXFLAGS += -std=c++11

CONFIG -= debug debug_and_release release

