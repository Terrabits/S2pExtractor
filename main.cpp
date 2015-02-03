

// Project
#include "Settings.h"
#include "Data.h"
#include "mainwindow.h"
#include "getFilenamesDialog.h"

// RsaToolbox
#include "Definitions.h"
#include "General.h"
#include "About.h"
#include "Keys.h"
#include "Log.h"
#include "Vna.h"
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QString>
#include <QApplication>
#include <QMessageBox>


bool isAboutMenu(int argc, char *argv[]);
bool isNoConnection(Vna &vna);
bool isUnknownModel(Vna &vna);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (isAboutMenu(argc, argv))
        return 0;

    Log log(LOG_FILENAME, APP_NAME, APP_VERSION);
    log.printHeader();

    Vna vna(CONNECTION_TYPE, INSTRUMENT_ADDRESS);
    vna.useLog(&log);
    vna.printInfo();

    Keys keys(KEY_PATH);

    if (isNoConnection(vna) || isUnknownModel(vna))
        return(0);

    Data data;
    data.setKeys(&keys);
    data.setVna(&vna);

    MainWindow w(&data);
    w.setWindowFlags(w.windowFlags() | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
    w.show();
    return a.exec();
}


bool isAboutMenu(int argc, char *argv[]) {
    if (argc != 2)
        return false;

    QString arg(argv[1]);
    arg = arg.trimmed().toUpper();
    if (arg == "-ABOUT" || arg == "--ABOUT") {
        Q_INIT_RESOURCE(AboutResources);
        About about;
        about.setAppName(APP_NAME);
        about.setVersion(APP_VERSION);
        about.setDescription(APP_DESCRIPTION);
        about.setContactInfo(CONTACT_INFO);
        about.exec();
        return true;
    }

    return false;
}
bool isNoConnection(Vna &vna) {
    if (vna.isDisconnected()) {
        QString message
                = "Please run this application on a Rohde & Schwarz VNA.";
        QMessageBox::critical(NULL,
                              "Instrument not found!",
                              message);
        vna.print(message);
        return(true);
    }
    else
        return(false);
}
bool isUnknownModel(Vna &vna) {
    if (vna.properties().isUnknownModel()) {
        QString message("Please use %1 with a Rohde & Schwarz instrument");
        message = message.arg(APP_NAME);
        QMessageBox::critical(NULL,
                              "VNA not recognized!",
                              message);
        vna.print(message);
        return(true);
    }
    else
        return(false);
}

