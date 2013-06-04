QT += widgets core gui

TARGET = Hussein
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/ui/mainwindow.cpp \
    src/ui/tasktreewidget.cpp \
    src/ui/tasktreeview.cpp


HEADERS  += \
    src/ui/mainwindow.h \
    src/ui/tasktreewidget.h \
    src/ui/tasktreeview.h

FORMS    += \
    src/ui/mainwindow.ui \
    src/ui/tasktreewidget.ui

OTHER_FILES += \
    Hussein.rc \
    Hussein.ico

RESOURCES += \
    Hussein.qrc

win32:RC_FILE += Hussein.rc
