ADS_OUT_ROOT = $${OUT_PWD}/../..

QT       += core gui widgets

TARGET = CentralWidgetExample
DESTDIR = $${ADS_OUT_ROOT}/lib
TEMPLATE = app
CONFIG += c++1z
CONFIG += debug_and_release
adsBuildStatic {
    DEFINES += ADS_STATIC
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

RC_ICONS = res\TextEdit.ico

LIBS += \
    -L$${ADS_OUT_ROOT}/lib \
    -lScintillaEdit
    

# Dependency: AdvancedDockingSystem (shared)
CONFIG(debug, debug|release){
    win32 {
        LIBS += -lqtadvanceddockingd
    }
    else:mac {
        LIBS += -lqtadvanceddocking_debug
    }
    else {
        LIBS += -lqtadvanceddocking
    }
}
else{
    LIBS += -lqtadvanceddocking
}

INCLUDEPATH += \
    ../../src \
    ../../scintilla/include \
    ../../scintilla/src \
    ../../scintilla/qt/ScintillaEditBase \
    ../../scintilla/qt/ScintillaEdit \

DEPENDPATH += ../../src

RESOURCES += \
    resources.qrc

