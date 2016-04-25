/********************************************************************************
** Form generated from reading UI file 'getFilenamesDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETFILENAMESDIALOG_H
#define UI_GETFILENAMESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ErrorLabel.h"

QT_BEGIN_NAMESPACE

class Ui_getFilenamesDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *filenamesLabel;
    QTableWidget *filenames;
    QLabel *label;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *directoryLabel;
    QLineEdit *directoryEdit;
    QPushButton *directoryButton;
    RsaToolbox::ErrorLabel *error;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *getFilenamesDialog)
    {
        if (getFilenamesDialog->objectName().isEmpty())
            getFilenamesDialog->setObjectName(QStringLiteral("getFilenamesDialog"));
        getFilenamesDialog->resize(350, 370);
        verticalLayout = new QVBoxLayout(getFilenamesDialog);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(15, 15, 15, 15);
        filenamesLabel = new QLabel(getFilenamesDialog);
        filenamesLabel->setObjectName(QStringLiteral("filenamesLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        filenamesLabel->setFont(font);

        verticalLayout->addWidget(filenamesLabel);

        filenames = new QTableWidget(getFilenamesDialog);
        if (filenames->columnCount() < 2)
            filenames->setColumnCount(2);
        filenames->setObjectName(QStringLiteral("filenames"));
        filenames->setFocusPolicy(Qt::NoFocus);
        filenames->setTabKeyNavigation(false);
        filenames->setProperty("showDropIndicator", QVariant(false));
        filenames->setDragDropOverwriteMode(false);
        filenames->setSelectionMode(QAbstractItemView::NoSelection);
        filenames->setColumnCount(2);
        filenames->horizontalHeader()->setVisible(false);
        filenames->horizontalHeader()->setStretchLastSection(true);
        filenames->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(filenames);

        label = new QLabel(getFilenamesDialog);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setItalic(true);
        label->setFont(font1);

        verticalLayout->addWidget(label);

        widget = new QWidget(getFilenamesDialog);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(4);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 14, 0, 0);
        directoryLabel = new QLabel(widget);
        directoryLabel->setObjectName(QStringLiteral("directoryLabel"));
        directoryLabel->setFont(font);

        gridLayout->addWidget(directoryLabel, 0, 0, 1, 2);

        directoryEdit = new QLineEdit(widget);
        directoryEdit->setObjectName(QStringLiteral("directoryEdit"));
        directoryEdit->setFocusPolicy(Qt::NoFocus);
        directoryEdit->setReadOnly(true);

        gridLayout->addWidget(directoryEdit, 1, 0, 1, 1);

        directoryButton = new QPushButton(widget);
        directoryButton->setObjectName(QStringLiteral("directoryButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(directoryButton->sizePolicy().hasHeightForWidth());
        directoryButton->setSizePolicy(sizePolicy);
        directoryButton->setMinimumSize(QSize(31, 28));
        directoryButton->setMaximumSize(QSize(31, 28));

        gridLayout->addWidget(directoryButton, 1, 1, 1, 1);


        verticalLayout->addWidget(widget);

        error = new RsaToolbox::ErrorLabel(getFilenamesDialog);
        error->setObjectName(QStringLiteral("error"));
        error->setMinimumSize(QSize(0, 16));

        verticalLayout->addWidget(error);

        verticalSpacer = new QSpacerItem(20, 19, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(getFilenamesDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setFocusPolicy(Qt::StrongFocus);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(filenames, directoryButton);
        QWidget::setTabOrder(directoryButton, buttonBox);

        retranslateUi(getFilenamesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), getFilenamesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), getFilenamesDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(getFilenamesDialog);
    } // setupUi

    void retranslateUi(QDialog *getFilenamesDialog)
    {
        getFilenamesDialog->setWindowTitle(QApplication::translate("getFilenamesDialog", "Dialog", 0));
        filenamesLabel->setText(QApplication::translate("getFilenamesDialog", "File names:", 0));
        label->setText(QApplication::translate("getFilenamesDialog", "*File extensions are added automatically", 0));
        directoryLabel->setText(QApplication::translate("getFilenamesDialog", "Directory:", 0));
        directoryButton->setText(QApplication::translate("getFilenamesDialog", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class getFilenamesDialog: public Ui_getFilenamesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETFILENAMESDIALOG_H
