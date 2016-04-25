/********************************************************************************
** Form generated from reading UI file 'getPortsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETPORTSDIALOG_H
#define UI_GETPORTSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "ErrorLabel.h"

QT_BEGIN_NAMESPACE

class Ui_getPortsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *ports;
    QCheckBox *selectAll;
    RsaToolbox::ErrorLabel *error;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *getPortsDialog)
    {
        if (getPortsDialog->objectName().isEmpty())
            getPortsDialog->setObjectName(QStringLiteral("getPortsDialog"));
        getPortsDialog->resize(350, 325);
        verticalLayout = new QVBoxLayout(getPortsDialog);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(15, 15, 15, 15);
        ports = new QListWidget(getPortsDialog);
        ports->setObjectName(QStringLiteral("ports"));
        ports->setSelectionMode(QAbstractItemView::NoSelection);

        verticalLayout->addWidget(ports);

        selectAll = new QCheckBox(getPortsDialog);
        selectAll->setObjectName(QStringLiteral("selectAll"));

        verticalLayout->addWidget(selectAll);

        error = new RsaToolbox::ErrorLabel(getPortsDialog);
        error->setObjectName(QStringLiteral("error"));
        error->setMinimumSize(QSize(0, 16));

        verticalLayout->addWidget(error);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(getPortsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(ports, buttonBox);

        retranslateUi(getPortsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), getPortsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), getPortsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(getPortsDialog);
    } // setupUi

    void retranslateUi(QDialog *getPortsDialog)
    {
        getPortsDialog->setWindowTitle(QApplication::translate("getPortsDialog", "Choose Ports...", 0));
        selectAll->setText(QApplication::translate("getPortsDialog", "Select All", 0));
    } // retranslateUi

};

namespace Ui {
    class getPortsDialog: public Ui_getPortsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETPORTSDIALOG_H
