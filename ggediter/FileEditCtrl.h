#pragma once

#include <ScintillaEdit.h>
#include "DockWidget.h"

class FileEditCtrl : public ScintillaEdit {
	Q_OBJECT

public:
	void Init();
	FileEditCtrl(QWidget *parent = 0);
	virtual ~FileEditCtrl();

	QByteArray GetAllText();

	QString& GetFilePath() { return _filePath; }

	void SetFilePath(QString& filePath);
	void SetFileName(QString & fileName) {
		auto oldName = _fileName;
		_fileName = fileName;
		if (dockWidget) {
			dockWidget->setWindowTitle(_fileName);
		}

		auto oldExtName = ExtName(oldName).toLower();
		auto newExtName = ExtName(fileName).toLower();
		if (newExtName != oldExtName) {
			Color(newExtName);
		}

	}

	bool IsNew() {
		return _filePath.isEmpty();
	}

    void Save();

	ads::CDockWidget* dockWidget = nullptr;
private:
	
    void Color(const QString & extName);

	QString ExtName(const QString & fileName) {
		auto pos = fileName.lastIndexOf('.');
		if (pos == -1) {
			return "";
		}

		return fileName.mid(pos + 1);
	}

	QString _filePath;
	QString _fileName;

    // have not saved content
    bool _isChanging;

public slots:
    void OnContentChange();

};
