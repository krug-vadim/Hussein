QT += widgets core gui

TARGET = Hussein
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/ui/mainwindow.cpp

HEADERS  += \
    src/ui/mainwindow.h

FORMS    += \
    src/ui/mainwindow.ui

OTHER_FILES += \
    Hussein.rc \
    Hussein.ico

RESOURCES += \
    Hussein.qrc

win32:RC_FILE += Hussein.rc
