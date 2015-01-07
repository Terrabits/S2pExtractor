===GENERATE===
if (!_isReadyToGenerate())
        return;

_constructX();
QVector<uint> ports = _portsDialog.selectedPorts();

QString directory;
directory = _vna.fileSystem().directory(VnaFileSystem::Directory::TRACES_DIRECTORY);
getFilenamesDialog filenameDialog(ports, directory, this);
filenameDialog.exec();
if (filenameDialog.isOkClicked()) {
    directory = filenameDialog.directory();
    QStringList filenames = filenameDialog.filenames();
    int size = filenames.size();
    for (int i = 0; i < size; i++) {
        NetworkData data = _calculateNetwork(ports[i]);
        QFileInfo file(QDir(directory), filenames[i]);
        Touchstone::Write(data, file.filePath());
    }
    _deleteChannels();
    close();
}
else {
    _deleteChannels();
}

===CONSTRUCTX===
if (_sweepType == VnaChannel::SweepType::Linear)
    linearSpacing(_x_Hz, _start_Hz, _stop_Hz, _points);
else if (_outerCorrections.sweepType() == VnaChannel::SweepType::Log)
    logSpacing(_x_Hz, _start_Hz, _stop_Hz, _points);
else
    // Cannot calculate frequency points!
    _x_Hz.clear();

===CALCULATE=== | uint port
uint otherPort;
if (_ports.first() == port)
    otherPort = _ports.last();
else
    otherPort = _ports.first();

ComplexRowVector ed, er, es;
ed = _outerCorrections.directivity(otherPort, port);
er = _outerCorrections.reflectionTracking(otherPort, port);
es = _outerCorrections.sourceMatch(otherPort, port);

ComplexRowVector edFix, erFix, esFix;
edFix = _innerCorrections.directivity(otherPort, port);
erFix = _innerCorrections.reflectionTracking(otherPort, port);
esFix = _innerCorrections.sourceMatch(otherPort, port);

ComplexRowVector denominator = add(er, multiplyEach(es, subtract(edFix, ed)));
ComplexRowVector s11 = divideEach(subtract(edFix, ed), denominator);
ComplexRowVector s22 = subtract(esFix, divideEach(multiplyEach(es, erFix), denominator));

ComplexRowVector s21Numerator = smoothSqrt(multiplyEach(er, erFix));
ComplexRowVector s21 = divideEach(s21Numerator, denominator);
s21 = smoothPhase(s21);
s21 = fixPhaseAtDc(_x_Hz, s21);

ComplexMatrix3D y;
_constructMatrix(y, s11, s21, s22);

NetworkData data;
data.setComment("");
data.setParameter(NetworkParameter::S);
data.setXUnits(Units::Hertz);
data.setData(_x_Hz, y);
return(data);