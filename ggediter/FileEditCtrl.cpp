#include "FileEditCtrl.h"
#include "SciLexer.h"
#include "LangModule.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "DockWidgetTab.h"

FileEditCtrl::FileEditCtrl(QWidget *parent)
	:ScintillaEdit(parent)
{
    connect(this, SIGNAL(linesAdded(int )), this, SLOT(OnLinesAdded(int )));
    connect(this, SIGNAL(charAdded(int )), this, SLOT(OnCharAdded(int )));
    connect(this, SIGNAL(modified(int , int , int , int ,
                                  const QByteArray &, int , int , int )),
            this, SLOT(OnModified(int , int , int , int ,
                                  const QByteArray &, int , int , int )));
}

FileEditCtrl::~FileEditCtrl() {

}

void FileEditCtrl::Init() {
    // 2号页边，宽度为20，显示行号
    setMarginTypeN(0, SC_MARGIN_NUMBER);
    setMarginWidthN(0, 30);
    setMarginBackN(SC_MARGIN_NUMBER, 0x001515A3);
    setCodePage(SC_CP_UTF8);

    // 当前行高亮
    setCaretLineVisible(true);
    setCaretLineBack(0xb0ffff);

    auto tabWidget = (ads::CDockWidgetTab*)dockWidget->tabWidget();
    if(IsNew()){
        _isChanging = true;
        tabWidget->setFontColor("red");
    }else{
        tabWidget->setFontColor("black");
    }

    _changeTimes = 0;
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

    

    // tab键宽度
    setTabWidth(4);
}


void FileEditCtrl::OnLinesAdded(int linesAdded)
{
//    qDebug() << "FileEditCtrl::OnLinesAdded";
//    if(!_isChanging){
//        _isChanging = true;
//        auto tabWidget = (ads::CDockWidgetTab*)dockWidget->tabWidget();
//        tabWidget->setFontColor("red");
//    }
}

void FileEditCtrl::OnCharAdded(int ch){
//    qDebug() << "FileEditCtrl::OnCharAdded";
//    if(!_isChanging){
//        _isChanging = true;
//        auto tabWidget = (ads::CDockWidgetTab*)dockWidget->tabWidget();
//        tabWidget->setFontColor("red");
//    }
}

void FileEditCtrl::OnModified(int type, int position, int length, int linesAdded,
              const QByteArray &text, int line, int foldNow, int foldPrev)
{
    if( (type & SC_MOD_INSERTTEXT) || (type & SC_MOD_DELETETEXT)){
        if(_changeTimes++>0){
            qDebug() << "FileEditCtrl::OnModified";
            if(!_isChanging){
                _isChanging = true;
                auto tabWidget = (ads::CDockWidgetTab*)dockWidget->tabWidget();
                tabWidget->setFontColor("red");
            }
        }
    }

}

void FileEditCtrl::Save()
{
    if(!_isChanging){
        return;
    }

    _isChanging = false;
    auto tabWidget = (ads::CDockWidgetTab*)dockWidget->tabWidget();
    tabWidget->setFontColor("black");

    auto edit = this;
    auto arr = edit->GetAllText();
    QString strFilePath = edit->GetFilePath();
    if (strFilePath.isEmpty()) {
        // new file
        strFilePath = QFileDialog::getSaveFileName(this, "Save", "", "*.*");
    }

    if (strFilePath.isEmpty()) {
        return;
    }

    edit->SetFilePath(strFilePath);

    QFile file(strFilePath);
    if (!file.open(QFile::WriteOnly)) {
        QString tip = edit->GetFilePath() + "open faild!";
        qDebug() << edit->GetFilePath() << "open faild!\n";
        QMessageBox::warning(this, "save error", tip, QMessageBox::Ok);
        return;
    }

    file.write(arr);
    file.close();


}
