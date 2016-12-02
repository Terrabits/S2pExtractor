#ifndef ERROR_H
#define ERROR_H

// Qt
#include <QMetaType>
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

Q_DECLARE_METATYPE(Error::Code)

#endif // ERROR_H
