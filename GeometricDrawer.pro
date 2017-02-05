#-------------------------------------------------
#
# Project created by QtCreator 2014-07-22T22:10:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeometricDrawer
TEMPLATE = app


SOURCES += main.cpp\
        View/mainwindow.cpp \
    View/renderarea.cpp \
    Model/form.cpp \
    utils.cpp \
    View/drawarea.cpp \
    View/editarea.cpp \
    Model/mainmodel.cpp \
    Controller/maincontroller.cpp \
    Model/guide.cpp

HEADERS  += View/mainwindow.h \
    View/renderarea.h \
    Model/form.h \
    utils.h \
    View/drawarea.h \
    View/editarea.h \
    Model/mainmodel.h \
    Controller/maincontroller.h \
    Model/guide.h

FORMS    += View/mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    Resources.qrc
