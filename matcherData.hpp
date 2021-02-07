#ifndef MATCHERDATA_H
#define MATCHERDATA_H

// QT
#include <QString>

struct matcherData_t final
{
    matcherData_t() = default;
    explicit matcherData_t(QString &name)
        : name(name)
    {}

    QString id;
    QString name;
};

#endif // MATCHERDATA_H
