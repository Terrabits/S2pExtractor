#include "Error.h"


QString Error::toString(Code code) {
    switch(code) {
    case Code::OuterCalibration:
        return "Outer cal";
    case Code::InnerCalibration:
        return "Inner cal";
    case Code::Ports:
        return "Ports";
    case Code::Other:
        return "Error";
    case Code::None:
        return "No Error";
    default:
        return "Error";
    }
}

Error::Error()
{
    clear();
}

Error::~Error()
{

}

bool Error::isError() const {
    return code != Code::None;
}
void Error::clear() {
    code = Code::None;
    message.clear();
}

QString Error::display() const {
    if (!isError()) {
        return "No Error";
    }
    if (code == Code::Other) {
        return message;
    }

    QString s = "%1: %2";
    s = s.arg(toString(code));
    s = s.arg(message);
    return s;
}
