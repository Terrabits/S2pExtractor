

// Project
#include "Settings.h"
#include "CalDialog.h"
#include "CalWidget.h"

// RsaToolbox
#include <Keys.h>
#include <Log.h>
#include <Vna.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QString>
#include <QApplication>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Keys keys(KEY_PATH);

    Log log(LOG_FILENAME, APP_NAME, APP_VERSION);
    log.printHeader();

    Vna vna(CONNECTION_TYPE, INSTRUMENT_ADDRESS);
    vna.useLog(&log);
    vna.printInfo();

//    QStringList calGroups;
//    calGroups << "cal group 1"
//              << "cal group 2.cal"
//              << "cal group 3";

//    QVector<uint> channels;
//    channels << 1 << 2 << 3;

//    CalDialog calDialog(calGroups, channels);
//    calDialog.exec();
//    qDebug() << "Selected: " << calDialog.source().displayText();

    CalWidget widget;
    widget.setVna(&vna);
    widget.show();
    return a.exec();
}
