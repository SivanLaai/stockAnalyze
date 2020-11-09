#-------------------------------------------------
#
# Project created by QtCreator 2017-03-28T21:29:53
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StockAnalyze
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    image_widget.cpp \
    info_widget.cpp \
    option_widget.cpp \
    search_widget.cpp \
    title_logo.cpp \
    title_menu.cpp \
    title_quitbox_widget.cpp \
    title_slots.cpp \
    title_widget.cpp \
    mouse.cpp \
    frameless.cpp \
    time.cpp \
    search_person_widget.cpp \
    person_dialog.cpp \
    person_dialog_slot.cpp \
    login_dialog.cpp \
    register_dialog.cpp \
    image_line_show.cpp \
    stockDataGet.cpp \
    painter.cpp \
    dataCal.cpp \
    # webgets.cpp \
    completelineedit.cpp \
    image_info_show.cpp \
    selectstockwidget.cpp \
    qwebdata.cpp \
    selectstock_widget.cpp

HEADERS  += mainwindow.h \
    frameless.h \
    login_dialog.h \
    register_dialog.h \
    image_line_show.h \
    stockDataGet.h \
    painter.h \
    dataCal.h \
    # webgets.h \
    opstring.h \
    completelineedit.h \
    image_info_show.h \
    selectstockwidget.h \
    qwebdata.h

FORMS    += mainwindow.ui \
    login_dialog.ui \
    register_dialog.ui \
    image_line_show.ui \
    selectstockwidget.ui

DISTFILES +=

RESOURCES += \
    imgrec.qrc

INCLUDEPATH += include\curl

# LIBS += F:\Desktop\StockAnalyze\lib\libcurldll.a
