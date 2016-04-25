/********************************************************************************
** Form generated from reading UI file 'getCalibrationDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETCALIBRATIONDIALOG_H
#define UI_GETCALIBRATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ErrorLabel.h"

QT_BEGIN_NAMESPACE

class Ui_getCalibrationDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *channelLabel;
    QListWidget *channels;
    QSpacerItem *verticalSpacer_2;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *calGroupLabel;
    QListWidget *calGroups;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_3;
    RsaToolbox::ErrorLabel *error;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *getCalibrationDialog)
    {
        if (getCalibrationDialog->objectName().isEmpty())
            getCalibrationDialog->setObjectName(QStringLiteral("getCalibrationDialog"));
        getCalibrationDialog->resize(350, 370);
        verticalLayout_3 = new QVBoxLayout(getCalibrationDialog);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(15, 15, 15, 15);
        widget = new QWidget(getCalibrationDialog);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        channelLabel = new QLabel(widget);
        channelLabel->setObjectName(QStringLiteral("channelLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        channelLabel->setFont(font);

        verticalLayout->addWidget(channelLabel);

        channels = new QListWidget(widget);
        channels->setObjectName(QStringLiteral("channels"));
        channels->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        channels->setEditTriggers(QAbstractItemView::NoEditTriggers);
        channels->setAlternatingRowColors(false);
        channels->setSelectionBehavior(QAbstractItemView::SelectItems);

        verticalLayout->addWidget(channels);


        verticalLayout_3->addWidget(widget);

        verticalSpacer_2 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_2);

        widget_2 = new QWidget(getCalibrationDialog);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setSpacing(4);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        calGroupLabel = new QLabel(widget_2);
        calGroupLabel->setObjectName(QStringLiteral("calGroupLabel"));
        calGroupLabel->setFont(font);

        verticalLayout_2->addWidget(calGroupLabel);

        calGroups = new QListWidget(widget_2);
        calGroups->setObjectName(QStringLiteral("calGroups"));
        calGroups->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout_2->addWidget(calGroups);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);


        verticalLayout_3->addWidget(widget_2);

        verticalSpacer_3 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_3);

        error = new RsaToolbox::ErrorLabel(getCalibrationDialog);
        error->setObjectName(QStringLiteral("error"));
        error->setMinimumSize(QSize(0, 16));

        verticalLayout_3->addWidget(error);

        verticalSpacer = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(getCalibrationDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_3->addWidget(buttonBox);

        QWidget::setTabOrder(channels, calGroups);
        QWidget::setTabOrder(calGroups, buttonBox);

        retranslateUi(getCalibrationDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), getCalibrationDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), getCalibrationDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(getCalibrationDialog);
    } // setupUi

    void retranslateUi(QDialog *getCalibrationDialog)
    {
        getCalibrationDialog->setWindowTitle(QApplication::translate("getCalibrationDialog", "Choose Calibration...", 0));
        channelLabel->setText(QApplication::translate("getCalibrationDialog", "Calibrated Channels:", 0));
        calGroupLabel->setText(QApplication::translate("getCalibrationDialog", "Saved Calibrations:", 0));
    } // retranslateUi

};

namespace Ui {
    class getCalibrationDialog: public Ui_getCalibrationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETCALIBRATIONDIALOG_H
