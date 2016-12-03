#ifndef GETCALDIALOG_H
#define GETCALDIALOG_H


// Project
#include "CalibrationSource.h"

// Qt
#include <QDialog>
#include <QStringList>
#include <QVector>


namespace Ui {
class getCalDialog;
}

class CalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalDialog(const QStringList &calGroups = QStringList(),
                       const QVector<uint> &channels = QVector<uint>(),
                       QWidget *parent = 0);
    ~CalDialog();

    CalibrationSource source() const;
    void setSource(CalibrationSource source);

public slots:
    virtual void accept();
    virtual void reject();

private:
    Ui::getCalDialog *ui;

    QStringList   _calGroups;
    QVector<uint> _channels;
    void setCalGroupsList(const QStringList &groups);
    void setChannelsList(const QVector<uint> &channels);

    bool isCalGroupTab    () const;
    void addCalGroupTab   ();
    void removeCalGroupTab();

    bool isChannelTab    () const;
    void addChannelTab   ();
    void removeChannelTab();

    void updateCalGroups();
    void updateChannels ();

    CalibrationSource selectedSource();
    CalibrationSource _source;
    void clearSelection();
    void selectSource();
};

#endif // GETCALDIALOG_H
