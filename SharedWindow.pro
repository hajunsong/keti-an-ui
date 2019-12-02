QT += core gui widgets printsupport

CONFIG += c++11
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    stepthread.cpp \
    qcustomplot.cpp \
    qclicklabel.cpp \
    qroundprogressbar.cpp \

HEADERS += \
    mainwindow.h \
    stepthread.h \
    qcustomplot.h \
    qclicklabel.h \
    qroundprogressbar.h \

FORMS += \
    mainwindow.ui

RESOURCES += \
    images/images.qrc


