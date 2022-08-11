TEMPLATE        = lib
CONFIG         += plugin c++17
DEFINES        += QT_DEPRECATED_WARNINGS
QT             += quick
TARGET          = karunit_assistant_plugin
DESTDIR         = $$PWD/../karunit/app/plugins

unix {
target.path = /usr/local/bin/plugins
INSTALLS += target
}

libspeechrecognitor.target = $$PWD/third-party/speechrecognitor/build/lib/libspeechrecognitor.so
libspeechrecognitor.commands += \
    mkdir -p $$PWD/third-party/speechrecognitor/build \
    && cd $$PWD/third-party/speechrecognitor/build \
    && cmake .. \
    && make -j4
QMAKE_EXTRA_TARGETS += libspeechrecognitor
PRE_TARGETDEPS += $$libspeechrecognitor.target

libspeechrecognitor.files = $$PWD/third-party/speechrecognitor/build/lib/libspeechrecognitor.so*
libspeechrecognitor.path = /usr/lib
INSTALLS += libspeechrecognitor

LIBS += -L$$PWD/third-party/speechrecognitor/build/lib -lspeechrecognitor
INCLUDEPATH += $$PWD/third-party/speechrecognitor/lib/include

LIBS += -L$$PWD/../karunit/plugininterface/ -lkarunit_plugininterface
INCLUDEPATH += $$PWD/../karunit/plugininterface

LIBS += -L$$PWD/../karunit/common/ -lkarunit_common
INCLUDEPATH += $$PWD/../karunit/common

LIBS += -L$$PWD/../karunit/third-party/xblog/lib -lxblog
INCLUDEPATH += $$PWD/../karunit/third-party/xblog/include

SUBDIRS += \
    src/ \
    res/

include(src/src.pri)
include(res/res.pri)
