#ifndef ERROR_H
#define ERROR_H

// Qt
#include <QString>


struct Error
{
    enum /*class*/ Code {
        OuterCalibration,
        InnerCalibration,
        Ports,
        Other,
        None
    };
    static QString toString(Code code);


    Error();
    ~Error();

    Code    code;
    QString message;

    bool isError() const;
    void clear();

    QString display() const;
};

#endif // ERROR_H
