#ifndef FILENAMEDELEGATE_H
#define FILENAMEDELEGATE_H

#include <QStyledItemDelegate>

class FilenameDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FilenameDelegate(QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // FILENAMEDELEGATE_H
