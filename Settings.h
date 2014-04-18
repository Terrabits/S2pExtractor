#ifndef SETTINGS_H
#define SETTINGS_H


// RsaToolbox
#include "General.h"
#include "GenericBus.h"


// Application settings
const QString APP_NAME = "RSA S2P Extractor";
const QString APP_VERSION = "1.1.4"; //http://semver.org/
const QString APP_FOLDER = "RSA S2P Extractor";

// Log, Key
#ifdef DEBUG_MODE
const QString LOG_PATH = SOURCE_DIR;
const QString LOG_FILENAME = "DEBUG_LOGFILE.txt";
const QString KEY_PATH = QString(SOURCE_DIR) + "/Keys/";
#else
const QString LOG_PATH = RsaToolbox::GetAppDataPath(APP_FOLDER);
const QString LOG_FILENAME = "RSA Deembed Log.txt";
const QString KEY_PATH = LOG_PATH + "/Settings/";
#endif

// Connection settings
const RsaToolbox::ConnectionType CONNECTION_TYPE = RsaToolbox::TCPIP_CONNECTION;
const QString INSTRUMENT_ADDRESS = "127.0.0.1";
const unsigned int TIMEOUT_MS = 1000;

// Keys
const QString EXAMPLE_KEY = "EXAMPLE"; // Place your keys here


#endif

