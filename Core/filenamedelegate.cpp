#include "filenamedelegate.h"

#include <QLineEdit>
#include <QRegExpValidator>
#include <QStringBuilder>

FilenameDelegate::FilenameDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *FilenameDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QWidget *editor = QStyledItemDelegate::createEditor(parent, option, index);
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit) {
        const QString onlyChar   = "[\\w-\\\\(\\)\\[\\]&%#@! ]";
        const QString endChar    = "[\\w-\\\\(\\)\\[\\]&%#@! ]";
        const QString middleChars = "[\\w-\\.\\(\\)\\[\\]&%#@! ]*";
        QRegExp regex(onlyChar % "|(" % endChar % middleChars % endChar % ")");
        QValidator *v = new QRegExpValidator(regex, lineEdit);
        lineEdit->setValidator(v);
    }
    return editor;
}
