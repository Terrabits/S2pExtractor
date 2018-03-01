#include "savedirectory.h"

#include <QDir>
#include <QFileDialog>

SaveDirectory::SaveDirectory(QWidget *parent) : LabeledButton(parent)
{
    _lastPath = QDir::homePath();
    _path     = QDir::homePath();
    updateDisplay();
    connect(this, SIGNAL(buttonClicked()),
            this, SLOT(showDialog()));
}

QString SaveDirectory::path() const {
    return _path;
}
void SaveDirectory::setPath(QString path) {
    path = path.trimmed();
    if (path == _path) {
        return;
    }

    _path = path;
    updateDisplay();
    emit directoryChanged(_path);
}

void SaveDirectory::showDialog() {
    QString response =
        QFileDialog::getExistingDirectory(
                topLevelWidget(),
                "Save to...",
                _lastPath.toString());
    if (response.isEmpty()) {
        // User canceled
        return;
    }
    if (response == _path) {
        return;
    }

    _path = response;
    _lastPath.setPath(response);
    updateDisplay();
    emit directoryChanged(_path);
}

void SaveDirectory::updateDisplay() {
    setLabel(_path.replace("/", "\\"));
}
