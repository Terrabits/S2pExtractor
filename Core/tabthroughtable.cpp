#include "tabthroughtable.h"

#include <QDebug>
#include <QKeyEvent>
#include <QLineEdit>

TabThroughTable::TabThroughTable(QWidget *parent) : QTableWidget(parent)
{

}

bool TabThroughTable::firstItemFocused() const {
    return currentRow() == 0;
}
bool TabThroughTable::lastItemFocused() const {
    return currentRow() == rowCount()-1;
}

void TabThroughTable::focusInEvent(QFocusEvent *event) {
    switch (event->reason()) {
    case Qt::TabFocusReason:
        event->accept();
        setCurrentCell(0,0);
        selectRow(0);
        break;
    case Qt::BacktabFocusReason:
        event->accept();
        setCurrentCell(rowCount()-1, 0);
        selectRow(rowCount()-1);
        break;
    default:
        break;
    }
    QTableWidget::focusInEvent(event);
}
void TabThroughTable::focusOutEvent(QFocusEvent *event) {
    clearSelection();
    QTableWidget::focusOutEvent(event);
}

void TabThroughTable::keyPressEvent(QKeyEvent *event) {
    const int key = event->key();
    if (key == Qt::Key_Tab && lastItemFocused()) {
        event->accept();
        nextInFocusChain()->setFocus();
        return;
    }
    if (key == Qt::Key_Backtab && firstItemFocused()) {
        event->accept();
        previousInFocusChain()->setFocus();
        return;
    }
    QTableWidget::keyPressEvent(event);
}

void TabThroughTable::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint) {
    const int lastRow  = rowCount()-1;
    const int firstRow = 0;
    if (hint == QAbstractItemDelegate::EditNextItem && currentRow() == lastRow) {
        QWidget *nextWidget = editor->nextInFocusChain();
        QTableWidget::closeEditor(editor, QAbstractItemDelegate::NoHint);
        clearSelection();
        nextWidget->setFocus();
        return;
    }
    if (hint == QAbstractItemDelegate::EditPreviousItem && currentRow() == firstRow) {
        QTableWidget::closeEditor(editor, QAbstractItemDelegate::NoHint);
        clearSelection();
        previousInFocusChain()->setFocus();
        return;
    }
    QTableWidget::closeEditor(editor, hint);
}
