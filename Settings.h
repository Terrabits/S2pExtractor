#ifndef SETTINGS_H
#define SETTINGS_H


// RsaToolbox
#include "General.h"
#include "GenericBus.h"


// Application settings
const QString APP_NAME = "R&S S2P Extractor";
const QString APP_VERSION = "1.2";
const QString APP_DESCRIPTION = "Characterizes unmeasurable devices such as probes and test fixtures by subtracting specific calibrations";
const QString MANUFACTURER_FOLDER = "Rohde-Schwarz";
const QString APP_FOLDER = "S2P Extractor";
const QString CONTACT_INFO = "<html><head/><body><p>Nick Lalic<br/>VNA Software Developer<br/>Cupertino, CA USA<br/>+1 424 200 2846<br/>nick.lalic@rsa.rohde-schwarz.com<br/><a href=\"http://vna.rs-us.net\"><span style=\"text-decoration: underline; color:#0000ff;\">http://vna.rs-us.net</span></a></p></body></html>";

// Log, Key
#ifdef DEBUG_MODE
const QDir dataDir(SOURCE_DIR);
const QString LOG_FILENAME = dataDir.filePath("DEBUG_LOGFILE.txt");
const QString KEY_PATH = dataDir.filePath("Keys");
#else
const QDir dataDir(RsaToolbox::GetAppDataPath(MANUFACTURER_FOLDER, APP_FOLDER));
const QString LOG_FILENAME = dataDir.filePath("R&S S2P Extractor Log.txt");
const QString KEY_PATH = dataDir.filePath("Settings");
#endif

// Connection settings
const RsaToolbox::ConnectionType CONNECTION_TYPE = RsaToolbox::TCPIP_CONNECTION;
const QString INSTRUMENT_ADDRESS = "127.0.0.1";
const unsigned int TIMEOUT_MS = 1000;

// Keys
const QString OUTERCAL_KEY = "OuterCalibration";
const QString INNERCAL_KEY = "InnerCalibration";
const QString PORTS_KEY = "Ports";
const QString SAVE_PATH_KEY = "SavePath";
const QString FILENAMES_KEY = "Filenames";


#endif
