#-------------------------------------------------
#
# Project created by QtCreator 2015-09-02T02:48:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Minesweeper
TEMPLATE = app


SOURCES += main.cpp\
        minesweeperview.cpp \
    minesweepermodel.cpp \
    minesweepermodelqt.cpp \
    minesweepergameoverview.cpp \
    minesweeperwelcomeview.cpp

HEADERS  += minesweeperview.h \
    minesweepermodel.h \
    minesweepermodelqt.h \
    minesweepergameoverview.h \
    minesweeperwelcomeview.h \
    minesweepercommon.h

FORMS    += minesweeperview.ui \
    minesweepergameoverview.ui \
    minesweeperwelcomeview.ui

DISTFILES += \
    .gitignore \
    README.md

QMAKE_CXXFLAGS += -std=c++14 -stdlib=libc++ -W -Wall -Wextra -Werror
LIBS += -stdlib=libc++ -lc++abi
