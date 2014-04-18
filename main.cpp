

// RsaDeembed
#include "Settings.h"
#include "mainwindow.h"

// RsaToolbox
#include "Log.h"
#include "Vna.h"
#include "Key.h"
using namespace RsaToolbox;

// Qt
#include <QString>
#include <QApplication>
#include <QMessageBox>


bool isNoConnection(Vna &vna);
bool isUnknownModel(Vna &vna);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log log(LOG_PATH, LOG_FILENAME, APP_NAME, APP_VERSION);
    log.printApplicationHeader();

    Vna vna(CONNECTION_TYPE, INSTRUMENT_ADDRESS);
    vna.useLog(&log);
    vna.printInfo();

    Key key(KEY_PATH);

    if (isNoConnection(vna) || isUnknownModel(vna))
        return(0);

    MainWindow w(vna, key);
    w.setWindowFlags(w.windowFlags() | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
    w.show();
    return a.exec();
}


bool isNoConnection(Vna &vna) {
    if (vna.isDisconnected()) {
        QString error_message
                = "Instrument not found. Please run this application on a Rohde & Schwarz VNA.";
        QMessageBox::critical(NULL,
                              "RSA Deembed",
                              error_message);
        vna.print(error_message);
        return(true);
    }
    else
        return(false);
}
bool isUnknownModel(Vna &vna) {
    if (vna.properties().isUnknownModel()) {
        QString error_message(QString("VNA not recognized.\n")
                              + "Please use RSA Deembed with a Rohde & Schwarz instrument");
        QMessageBox::critical(NULL,
                              "RSA Deembed",
                              error_message);
        vna.print(error_message);
        return(true);
    }
    else
        return(false);
}

