ADS_OUT_ROOT = $${OUT_PWD}/../..

QT       += core gui widgets

TARGET = GGEditer
DESTDIR = $$PWD/../bin
TEMPLATE = app
CONFIG += c++1z
CONFIG += debug
adsBuildStatic {
    DEFINES += ADS_STATIC
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    LangModule.cpp \
    main.cpp \
    mainwindow.cpp \
    FileEditCtrl.cpp

HEADERS += \
    LangModule.h \
    mainwindow.h \
    FileEditCtrl.h

FORMS += \
    mainwindow.ui

RC_ICONS = res\TextEdit.ico

LIBS += \
    -L../lib \
    -lScintillaEdit
    

# Dependency: AdvancedDockingSystem (shared)
CONFIG(debug, debug|release){
    win32 {
        LIBS += -lqtadvanceddocking
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
    ../src \
    ../scintilla/include \
    ../scintilla/src \
    ../scintilla/qt/ScintillaEditBase \
    ../scintilla/qt/ScintillaEdit \

DEPENDPATH += ../src

RESOURCES += \
    resources.qrc

