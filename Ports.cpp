#include "Ports.h"


SharedPorts newPorts() {
    return SharedPorts(new QVector<uint>());
}
