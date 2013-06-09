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
    src/limbs/task.cpp \
    src/serialization/yamlserialization.cpp \
    src/ui/tasktabwidget.cpp

HEADERS  += \
    src/models/taskmodel.h \
    src/ui/mainwindow.h \
    src/ui/tasktreewidget.h \
    src/ui/tasktreeview.h \
    src/limbs/taskfactory.h \
    src/serialization/jsonserialization.h \
    src/models/tasksortfilterproxymodel.h \
    src/limbs/task.h \
    src/serialization/yamlserialization.h \
    src/ui/tasktabwidget.h

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
    INCLUDEPATH += "lib/yaml-cpp/include"
    INCLUDEPATH += "lib/boost/"
}

CONFIG(debug, debug|release) {
    win32:LIBS += "-L$$PWD/lib/yaml-cpp/build/debug/"
    win32:LIBS += "-lyamld"
    unix:LIBS += -lyaml-cpp
} else {
    #win32:LIBS += -lyaml
    win32:LIBS += "-L$$PWD/lib/yaml-cpp/build/release/"
    win32:LIBS += "-lyaml"
    unix:LIBS += -lyaml-cpp
}
