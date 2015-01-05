#include "getFilenamesDialog.h"
#include "ui_getFilenamesDialog.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QString>
#include <QScopedPointer>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

getFilenamesDialog::getFilenamesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getFilenamesDialog)
{
    ui->setupUi(this);
    setWindowTitle("Save touchstone files...");

    _lastPath.setPath(QDir::homePath());
}
getFilenamesDialog::~getFilenamesDialog()
{
    delete ui;
}

bool getFilenamesDialog::isPorts() const {
    return !_ports.isNull();
}
SharedPorts getFilenamesDialog::ports() const {
    return _ports;
}
void getFilenamesDialog::setPorts(SharedPorts ports) {
    _ports = ports;
}

bool getFilenamesDialog::isKey() const {
    return _lastPath.isKey();
}
QString getFilenamesDialog::key() const {
    return _lastPath.key();
}
Keys *getFilenamesDialog::keys() const {
    return _lastPath.keys();
}
void getFilenamesDialog::setKey(Keys *keys, QString key) {
    _lastPath.setKey(keys, key);
}

QStringList getFilenamesDialog::filenames() const {
    return _filenames;
}
QString getFilenamesDialog::directory() const {
    return _directory;
}
QStringList getFilenamesDialog::filePathNames() const {
    QDir dir(_directory);
    QStringList _pathNames;
    foreach (QString filename, _filenames) {
        if (!filename.endsWith(".s2p", Qt::CaseInsensitive))
            filename += ".s2p";
        _pathNames << dir.filePath(filename);
    }
    return _pathNames;
}

int getFilenamesDialog::exec() {
    updateUi();

    int x = parentWidget()->geometry().x();
    x += parentWidget()->width()/2.0;
    x -= width()/2.0;

    int y = parentWidget()->geometry().y();
    y += parentWidget()->height()/2.0;
    y -= height()/2.0;

    this->setGeometry(x, y, width(), height());
    return QDialog::exec();
}
void getFilenamesDialog::accept() {
    if (_directory.isEmpty()) {
        QMessageBox::warning(this, "Invalid Directory...",
                             "Please choose a valid directory.");
        return;
    }

    _filenames.clear();
    for (int i = 0; i < _ports->size(); i++) {
        QLineEdit *edit;
        edit = qobject_cast<QLineEdit*>(ui->filenames->cellWidget(i, 1));
        QString filename = edit->text().trimmed();
        if (filename.isEmpty() || filename.remove(".").isEmpty()) {
            QMessageBox::warning(this, "Invalid filename...",
                                 "Please check the filenames and try again.");
            edit->selectAll();
            return;
        }
        _filenames.append(edit->text().trimmed());
    }
    QDialog::accept();
}

void getFilenamesDialog::on_directoryButton_clicked() {
    QString directory
            = QFileDialog::getExistingDirectory(this,
                                                "Choose directory...",
                                                _lastPath.toString());
    if (!directory.isEmpty()) {
        _directory = directory;
        ui->directoryEdit->setText(QDir::toNativeSeparators(_directory));
        _lastPath.setPath(_directory);
    }
}
void getFilenamesDialog::updateUi() {
    int size = _ports->size();
    QStringList header;
    header << "Port";
    header << "Filename";
    ui->filenames->setColumnCount(2);
    ui->filenames->setHorizontalHeaderLabels(header);
    ui->filenames->setColumnWidth(0, 30);
    ui->filenames->setRowCount(size);
    QString label = "%1";
    QString filename = "Port %1";
    QRegExp regExp("[^/?*:;{}\"\\<>|]+");
    for (int i = 0; i < size; i++) {
        uint port = _ports->at(i);
        QScopedPointer<QTableWidgetItem> item;
        item.reset(new QTableWidgetItem(label.arg(port)));
        item->setFlags(Qt::ItemIsEnabled);
        item->setBackground(palette().alternateBase());
        ui->filenames->setItem(i, 0, item.take());

        QScopedPointer<QLineEdit> editor;
        editor.reset(new QLineEdit(filename.arg(port), ui->filenames));
        editor->setValidator(new QRegExpValidator(regExp, editor.data()));
        editor->setFrame(false);
        ui->filenames->setCellWidget(i, 1, editor.take());
    }
}
