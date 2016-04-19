#ifndef GETFILENAMESDIALOG_H
#define GETFILENAMESDIALOG_H


// Rsatoolbox
#include "LastPath.h"
#include "Keys.h"

#include <QDialog>
#include <QVector>


namespace Ui {
class getFilenamesDialog;
}

class getFilenamesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getFilenamesDialog(QWidget *parent = 0);
    ~getFilenamesDialog();

    bool isPorts() const;
    QVector<uint> ports() const;
    void setPorts(QVector<uint> ports);

    bool isKey() const;
    QString key() const;
    RsaToolbox::Keys *keys() const;
    void setKey(RsaToolbox::Keys *keys, QString key);

    QStringList filenames() const;
    QString directory() const;
    QStringList filePathNames() const;

public slots:
    virtual int exec();
    virtual void accept();

private slots:
    void on_directoryButton_clicked();

private:
    Ui::getFilenamesDialog *ui;

    QVector<uint> _ports;
    RsaToolbox::LastPath _lastPath;

    QStringList _filenames;
    QString _directory;
    void updateUi();
};

#endif // GETFILENAMESDIALOG_H
