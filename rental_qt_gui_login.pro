QT += core gui widgets

TARGET = rental_qt_gui_login
TEMPLATE = app

SOURCES += main.cpp            mainwindow.cpp            rentalsystem.cpp            logindialog.cpp

HEADERS += mainwindow.h            rentalsystem.h            logindialog.h

FORMS += mainwindow.ui          logindialog.ui

CONFIG += c++17
