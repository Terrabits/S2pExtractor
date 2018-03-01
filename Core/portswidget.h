#ifndef PORTSWIDGET_H
#define PORTSWIDGET_H

#include <QAbstractItemDelegate>
#include <QList>
#include <QMap>
#include <QScopedPointer>
#include <QString>
#include <QVector>
#include <QWidget>

namespace Ui {
class PortsWidget;
}

class PortsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PortsWidget(QWidget *parent = 0);
    ~PortsWidget();

    QMap<uint, QString> filenames() const;

public slots:
    void setPorts(const QVector<uint> &ports);
    void setFilenames(const QMap<uint, QString> &filenames);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_table_cellChanged(int row, int column);

private:
    Ui::PortsWidget *ui;
    QScopedPointer<QAbstractItemDelegate> _delegate;

    void resizeTable();
    void setRowColor(const int row);
    void updateTable();

    QList<uint> ports() const;
    uint portAtRow(const int row) const;
    int rowForPort(const uint port) const;
    QMap<uint, QString> _filenames;

    QMap<uint, QString> _history;
    void updateHistory();
    void applyHistory();
    void applyDefaults();
};

#endif // PORTSWIDGET_H
