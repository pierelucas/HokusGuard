#ifndef MAILDATA_HPP
#define MAILDATA_HPP

// QT
#include <QString>

// STL
#include <memory>

// Project
#include "hosterData.hpp"

struct mailPass_t final
{
    mailPass_t() = default;
    explicit mailPass_t(QString &name, QString &mail, QString &pass, QString &hoster, QString &proxyip, QString &proxyport)
        : name(name)
        , mail(mail)
        , pass(pass)
        , hoster(hoster)
        , proxyip(proxyip)
        , proxyport(proxyport)
    {}
    ~mailPass_t() = default;

    QString id;
    QString name;
    QString mail;
    QString pass;
    QString hoster;
    std::shared_ptr<hoster_t> hosterCredentials;
    QString proxyip;
    QString proxyport;
};

struct mailData_t final
{
    mailData_t() = default;
    explicit mailData_t(QString &from, QString &to, QString &text)
        : from(from)
        , to(to)
        , text(text)
    {}
    ~mailData_t() = default;

    QString id;
    QString from;
    QString to;
    QString text;
};

#endif // MAILDATA_HPP
