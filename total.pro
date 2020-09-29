TEMPLATE = subdirs

SUBDIRS = \
	src \
        ggediter \
    src/qtadvanceddocking.pro \
    scintilla/qt/ScintillaEdit/ScintillaEdit.pro \
    scintilla/qt/ScintillaEditBase/ScintillaEditBase.pro


ggediter.depends = src
