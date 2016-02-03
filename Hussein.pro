QT += core gui widgets

TARGET = Hussein
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/models/taskmodel.cpp \
    src/ui/mainwindow.cpp \
    src/ui/tasktreewidget.cpp \
    src/ui/tasktreeview.cpp \
    src/models/tasksortfilterproxymodel.cpp \
    src/limbs/task.cpp \
    src/ui/tasktabwidget.cpp \
    src/limbs/taskmimedata.cpp \
    src/models/guitaskmodel.cpp \
    src/models/commands/taskremovecommand.cpp \
    src/models/commands/taskinsertcommand.cpp \
    src/models/commands/tasksetdatacommand.cpp \
    src/models/commands/taskmovecommand.cpp \
    src/serialization/jsonserializer.cpp

HEADERS  += \
    src/models/taskmodel.h \
    src/ui/mainwindow.h \
    src/ui/tasktreewidget.h \
    src/ui/tasktreeview.h \
    src/models/tasksortfilterproxymodel.h \
    src/limbs/task.h \
    src/ui/tasktabwidget.h \
    src/limbs/taskmimedata.h \
    src/models/guitaskmodel.h \
    src/models/commands/taskremovecommand.h \
    src/models/commands/taskinsertcommand.h \
    src/models/commands/tasksetdatacommand.h \
    src/models/commands/taskmovecommand.h \
    src/serialization/jsonserializer.h

FORMS    += \
    src/ui/mainwindow.ui \
    src/ui/tasktreewidget.ui

OTHER_FILES += \
    Hussein.rc \
    Hussein.ico

RESOURCES += \
    Hussein.qrc

win32 {
    RC_FILE += Hussein.rc
}
