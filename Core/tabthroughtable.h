#ifndef TABTHROUGHTABLE_H
#define TABTHROUGHTABLE_H

#include <QTableWidget>

class TabThroughTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit TabThroughTable(QWidget *parent = 0);

    bool firstItemFocused() const;
    bool lastItemFocused() const;



protected:
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);
};

#endif // TABTHROUGHTABLE_H
