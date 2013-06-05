QT += widgets core gui

TARGET = Hussein
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/models/taskmodel.cpp \
    src/ui/mainwindow.cpp \
    src/ui/tasktreewidget.cpp \
    src/ui/tasktreeview.cpp \
    src/limbs/taskfactory.cpp \
    src/serialization/jsonserialization.cpp \
    src/models/tasksortfilterproxymodel.cpp \
    src/limbs/task.cpp

HEADERS  += \
    src/models/taskmodel.h \
    src/ui/mainwindow.h \
    src/ui/tasktreewidget.h \
    src/ui/tasktreeview.h \
    src/limbs/taskfactory.h \
    src/serialization/jsonserialization.h \
    src/models/tasksortfilterproxymodel.h \
    src/limbs/task.h

FORMS    += \
    src/ui/mainwindow.ui \
    src/ui/tasktreewidget.ui

OTHER_FILES += \
    Hussein.rc \
    Hussein.ico

RESOURCES += \
    Hussein.qrc

win32:RC_FILE += Hussein.rc
