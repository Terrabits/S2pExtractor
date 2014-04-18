#ifndef GETFILENAMESDIALOG_H
#define GETFILENAMESDIALOG_H

#include <QDialog>
#include <QVector>

namespace Ui {
class getFilenamesDialog;
}

class getFilenamesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getFilenamesDialog(QVector<uint> ports, QString directory, QWidget *parent = 0);
    ~getFilenamesDialog();

    bool isOkClicked();
    QStringList filenames();
    QString directory();

private slots:
    void on_getFilenamesDialog_accepted();
    void on_directoryButton_clicked();

private:
    Ui::getFilenamesDialog *ui;

    int _numberOfPorts;

    bool _isOkClicked;
    QString _directory;
    QStringList _filenames;

    void _updateDirectory();
};

#endif // GETFILENAMESDIALOG_H
