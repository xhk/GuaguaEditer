#include "FileEditCtrl.h"
#include "SciLexer.h"
#include "LangModule.h"

FileEditCtrl::FileEditCtrl(QWidget *parent)
	:ScintillaEdit(parent)
{

}

FileEditCtrl::~FileEditCtrl() {

}

QByteArray FileEditCtrl::GetAllText() {
	auto length = textLength();
    QByteArray ba(length + 1, '\0');
    send(SCI_GETTEXT, length+1, (sptr_t)ba.data());
    // Remove extra NULs
    if (ba.size() > 0 && ba.at(ba.size() - 1) == 0)
        ba.chop(1);
    return ba;


}

void FileEditCtrl::SetFilePath(QString& filePath) {
	_filePath = filePath;
	if (!_filePath.isEmpty()) {
		QString strFileName = "";
		auto lastPos = filePath.lastIndexOf("/");
		if (lastPos != -1) {
			strFileName = filePath.mid(lastPos + 1);
		}
		else {
			strFileName = filePath;
		}

		SetFileName(strFileName);
	}
}

void FileEditCtrl::Color(const QString & extName)
{
    auto& langs = LangModule::Inst()._langs;

    Lang* pLang = nullptr;

    for (auto& lang : langs) {
        if (lang.ext.contains(extName)) {
            pLang = &lang;
        }
    }

    if (!pLang) {
        return;
    }

    setLexerLanguage(pLang->name.toStdString().c_str());
    //setLexer(SCLEX_CPP);

    styleClearAll();
    int kwIndex = 0;
    for (int i = 0; i < pLang->keywords.count(); ++i) {
        setKeyWords(kwIndex++, pLang->keywords[i].toStdString().c_str());
    }
    for (int i = 0; i < pLang->types.count(); ++i) {
        setKeyWords(kwIndex++, pLang->types[i].toStdString().c_str());
    }

    //w->setLexerLanguage("cpp");
    styleSetFore(SCE_C_WORD, 0x00FF0000);
    styleSetFore(SCE_C_WORD2, 0x00800080);
    styleSetBold(SCE_C_WORD2, true);
    styleSetFore(SCE_C_STRING, 0x001515A3);
    styleSetFore(SCE_C_CHARACTER, 0x001515A3);
    styleSetFore(SCE_C_PREPROCESSOR, 0x00808080);
    styleSetFore(SCE_C_COMMENT, 0x00008000);
    styleSetFore(SCE_C_COMMENTLINE, 0x00008000);
    styleSetFore(SCE_C_COMMENTDOC, 0x00008000);

    // 当前行高亮
    setCaretLineVisible(true);
    setCaretLineBack(0xb0ffff);

    // tab键宽度
    setTabWidth(4);
}