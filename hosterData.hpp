#ifndef HOSTERDATA_HPP
#define HOSTERDATA_HPP

// QT
#include <QString>

struct hoster_t final
{
    hoster_t() = default;
    explicit hoster_t(QString &name, QString &addr, QString &port)
        : name(name)
        , addr(addr)
        , port(port)
    {}
    QString id;
    QString name;
    QString addr;
    QString port;
};

#endif // HOSTERDATA_HPP
