#include "getFilenamesDialog.h"
#include "ui_getFilenamesDialog.h"

#include <QDebug>
#include <QString>
#include <QScopedPointer>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

getFilenamesDialog::getFilenamesDialog(QVector<uint> ports, QString directory, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getFilenamesDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    _isOkClicked = false;

    setWindowTitle("Save touchstone files...");

    _numberOfPorts = ports.size();
    QStringList header;
    header << "Port";
    header << "Filename";
    ui->table->setColumnCount(2);
    ui->table->setHorizontalHeaderLabels(header);
    ui->table->setColumnWidth(0, 98);
    ui->table->setColumnWidth(1, 200);
    ui->table->setRowCount(_numberOfPorts);
    QString label = "%1";
    QString filename = "Port %1";
    QRegExp regExp("[^/?*:;{}\"\\<>|]+");
    for (int i = 0; i < _numberOfPorts; i++) {
        uint port = ports[i];
        QScopedPointer<QTableWidgetItem> item;
        item.reset(new QTableWidgetItem(label.arg(port)));
        item->setFlags(Qt::ItemIsEnabled);
        item->setBackground(palette().alternateBase());
        ui->table->setItem(i, 0, item.take());

        QScopedPointer<QLineEdit> editor;
        editor.reset(new QLineEdit(filename.arg(port), ui->table));
        editor->setValidator(new QRegExpValidator(regExp, editor.data()));
        editor->setFrame(false);
        ui->table->setCellWidget(i, 1, editor.take());
    }

    _directory = directory;
    _updateDirectory();

    QObject::connect(ui->buttonBox, SIGNAL(accepted()),
                     this, SLOT(on_getFilenamesDialog_accepted()));
}

getFilenamesDialog::~getFilenamesDialog()
{
    delete ui;
}

bool getFilenamesDialog::isOkClicked() {
    return(_isOkClicked);
}
QStringList getFilenamesDialog::filenames() {
    return(_filenames);
}
QString getFilenamesDialog::directory() {
    return(_directory);
}

void getFilenamesDialog::on_getFilenamesDialog_accepted() {
    _filenames.clear();
    for (int i = 0; i < _numberOfPorts; i++) {
        QLineEdit *edit;
        edit = qobject_cast<QLineEdit*>(ui->table->cellWidget(i, 1));
        QString filename = edit->text().trimmed();
        if (filename.isEmpty() && filename.remove(".").isEmpty()) {
            QMessageBox::warning(this, "Invalid filename...",
                                 "Please check the filenames and try again.");
            edit->selectAll();
            return;
        }
        _filenames.append(edit->text().trimmed());
    }
    if (_directory.isEmpty()) {
        QMessageBox::warning(this, "Invalid Directory...",
                             "Please choose a valid directory.");
        return;
    }
    _isOkClicked = true;
    QObject::disconnect(this, SLOT(on_getFilenamesDialog_accepted()));
    setResult(QDialog::Accepted);
    emit accepted();
    close();
}

void getFilenamesDialog::on_directoryButton_clicked() {
    QString directory;
    directory = QFileDialog::getExistingDirectory(this, "Choose dir...", _directory);
    if (directory.isEmpty() == false) {
        this->_directory = directory;
        _updateDirectory();
    }
}
void getFilenamesDialog::_updateDirectory() {
    if (_directory.isEmpty() == false) {
        QString native = QDir::toNativeSeparators(_directory);
        ui->directoryEdit->setText(native);
    }
}
