#ifndef PORT_LOOP_H
#define PORT_LOOP_H


#include <QVector>


class PortLoop
{
public:
    PortLoop(QVector<uint> portsToProcess, QVector<uint> allPorts);
    ~PortLoop();

    bool isUnprocessedPorts() const;

    void begin();

    uint port1() const;

    bool isPort2Unprocessed() const;
    uint port2() const;

    void usePorts();
    bool next();
    uint percentComplete() const;
    void end();

private:
    QVector<uint> _unprocessedPorts;
    QVector<uint> _processedPorts;
    QVector<uint> _remainderPorts;

    bool isPortUnprocessed(uint port) const;
    void markPortProcessed(uint port);

    uint _port1;
    uint _port2;
    QVector<uint> _portList;
};

#endif // PORT_LOOP_H
