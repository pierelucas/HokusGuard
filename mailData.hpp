#ifndef MAILDATA_HPP
#define MAILDATA_HPP

// QT
#include <QString>

// STL
#include <memory>

struct mailPass_t final
{
    mailPass_t() = default;
    explicit mailPass_t(QString &name, QString &mail, QString &pass, QString &hoster, QString &hosterIMAPDomain, QString &hosterIMAPPort, QString &proxyip, QString &proxyport, QString &useproxy)
        : name(name)
        , mail(mail)
        , pass(pass)
        , hoster(hoster)
        , hosterIMAPDomain(hosterIMAPDomain)
        , hosterIMAPPort(hosterIMAPPort)
        , proxyip(proxyip)
        , proxyport(proxyport)
        , useproxy(useproxy)
    {}
    ~mailPass_t() = default;

    QString id;
    QString name;
    QString mail;
    QString pass;
    QString hoster;
    QString hosterIMAPDomain;
    QString hosterIMAPPort;
    QString proxyip;
    QString proxyport;
    QString useproxy; // Number value 0 = false 1 = true
};

struct mailData_t final
{
    mailData_t() = default;
    explicit mailData_t(QString &from, QString &to, QString &text, QString &date)
        : from(from)
        , to(to)
        , text(text)
        , date(date)
    {}
    ~mailData_t() = default;

    QString id;
    QString from;
    QString to;
    QString text;
    QString date;
};

#endif // MAILDATA_HPP
