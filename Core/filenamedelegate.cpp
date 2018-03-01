#include "filenamedelegate.h"

#include "filenames.h"

#include <QLineEdit>
#include <QRegExpValidator>

FilenameDelegate::FilenameDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *FilenameDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QWidget *editor = QStyledItemDelegate::createEditor(parent, option, index);
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit) {
        QValidator *v = new QRegExpValidator(filenameRegex(), lineEdit);
        lineEdit->setValidator(v);
    }
    return editor;
}
