#ifndef DATABASE_HPP
#define DATABASE_HPP

// QT
#include <QtSql>
#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <QFile>

// STL
#include <memory>

// Project
#include "mailData.hpp"
#include "hosterData.hpp"
#include "matcherData.hpp"

#define D_DBNAME "hokus.db"
#define D_CONNAME "default_connection"

class dbManager final : QWidget
{
public:
    dbManager();
    explicit dbManager(const QString conName);
    ~dbManager();
    dbManager& operator=(const dbManager&) = delete;
    dbManager(const dbManager&) = delete;

    // Getter && Setter
    void setPath(const QString &path);
    QString getPath() const;
    QString getConName() const;

    // CRUD Operations
    // TODO: Maybe change retrieveToVec from returning bool to returning vector
    bool addMatcher(const QString &name);
    bool delMatcher(const QString &name);
    bool delMatcher(const matcherData_t &m_t);
    bool retrieveToVec(std::vector<std::shared_ptr<matcherData_t>> &smart_vec);

    bool addMailPass(const QString &name, const QString &mail, const QString &pass, const QString &hoster, const QString &hosterIMAPDomain, const QString &hosterIMAPPort, const QString &proxyip, const QString &proxyport, const QString &useproxy);
    bool updateMailPass(const QString &name, const QString &mail, const QString &pass, const QString &hoster, const QString &hosterIMAPDomain, const QString &hosterIMAPPort, const QString &proxyip, const QString &proxyport, const QString &useproxy);
    bool delMailPass(const QString &name);
    bool delMailPass(const mailPass_t &mp_t);
    mailPass_t retrieveMailPassByName(const QString &name);
    bool retrieveToVec(std::vector<std::shared_ptr<mailPass_t>> &smart_vec);

    bool addHoster(const QString &name, const QString &addr, const QString &port);
    bool delHoster(const QString &name);
    bool delHoster(const hoster_t &h_t);
    bool retrieveToVec(std::vector<std::shared_ptr<hoster_t>> &smart_vec);

    bool addMail(const QString &from, const QString &to, const QString &text);
    bool delMail(const QString &id);
    bool delMail(const mailData_t &m_t);
    mailData_t retrieveMailById(const QString &id);
    bool retrieveToVec(std::vector<std::shared_ptr<mailData_t>> &smart_vec);

    bool clearDB();

private:
    // One argument constructor
    dbManager(const QString &path, const QString &conName);

    // Variables
    QSqlDatabase m_db;
    QString path;
    QString conName;

    // setup database
    void setupDB();
};

#endif // DATABASE_HPP
