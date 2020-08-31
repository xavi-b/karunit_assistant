TEMPLATE        = lib
CONFIG         += plugin c++17
DEFINES        += QT_DEPRECATED_WARNINGS
QT             += core widgets
TARGET          = karunit_assistant_plugin
DESTDIR         = $$PWD/../karunit/app/plugins

LIBS += -L$$PWD/../../plugininterface/ -lkarunit_plugininterface
INCLUDEPATH += $$PWD/../../plugininterface

LIBS += -L$$PWD/../../common/ -lkarunit_common
INCLUDEPATH += $$PWD/../../common

LIBS += -L$$PWD/../karunit/third-party/xblog/ -lxblog
INCLUDEPATH += $$PWD/../karunit/third-party/xblog/src

SUBDIRS += \
    src/

include(src/src.pri)

RESOURCES += \
    karunit_assistant.qrc
