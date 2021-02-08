#include "database.hpp"

#include "iostream"

dbManager::dbManager()
    : dbManager(D_DBNAME, D_CONNAME)
{}

dbManager::dbManager(const QString conName)
    : dbManager(D_DBNAME, conName)
{}

dbManager::dbManager(const QString &path, const QString &conName)
{
    this->path = path;
    this->conName = conName;
    this->setupDB();
}

dbManager::~dbManager()
{
    this->m_db.close();
}

void dbManager::setPath(const QString &path)
{
    this->path = path;
}

QString dbManager::getPath() const
{
    return this->path;
}

QString dbManager::getConName() const
{
    return this->m_db.connectionName();
}

void dbManager::setupDB()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", this->conName);
    m_db.setDatabaseName(this->path);

    if (!m_db.open())
    {
       QMessageBox::critical(this, "Database Error", "connection with database failed");
    }

    // Create Tables if not exists
    QSqlQuery qry(m_db);

    qry.prepare
    (
        "CREATE TABLE IF NOT EXISTS matcher ("
        "id integer UNIQUE PRIMARY KEY AUTOINCREMENT,"
        "name VARCHAR(100) UNIQUE )"
    );
    if ( !qry.exec() )
    {
        QMessageBox::critical(this, "Database Error", "corrupted database: C1");

    }

    qry.prepare
    (
        "CREATE TABLE IF NOT EXISTS mailpass ("
        "id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT,"
        "name VARCHAR(100) UNIQUE,"
        "mail VARCHAR(100) UNIQUE,"
        "pass VARCHAR(100),"
        "hoster VARCHAR(100),"
        "proxyip VARCHAR(100),"
        "proxyport VARCHAR(100),"
        "useproxy INTEGER )"
    );
    if ( !qry.exec() )
    {
        QMessageBox::critical(this, "Database Error", "corrupted database: C2");
    }

    qry.prepare
    (
        "CREATE TABLE IF NOT EXISTS hoster ("
        "id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT,"
        "name VARCHAR(100) UNIQUE,"
        "addr VARCHAR(100) UNIQUE,"
        "port VARCHAR(100) )"
    );
    if ( !qry.exec() )
    {
        QMessageBox::critical(this, "Database Error", "corrupted database: C3");
    }

    qry.prepare
    (
        "CREATE TABLE IF NOT EXISTS mail ("
        "id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT,"
        "froml VARCHAR(100),"
        "tol VARCHAR(100),"
        "textl VARCHAR(10000) )"
    );
    if ( !qry.exec() )
    {
        QMessageBox::critical(this, "Database Error", "corrupted database: C4");
    }
}

bool dbManager::clearDB()
{
    QSqlQuery qry(m_db);
    if (
         !qry.exec("DELETE FROM matcher;")  ||
         !qry.exec("DELETE FROM hoster;")   ||
         !qry.exec("DELETE FROM mailpass;") ||
         !qry.exec("DELETE FROM mail;") )
    {
        return false;
    }
    return true;
}

bool dbManager::addMatcher(const QString &name)
{
    QSqlQuery qry(m_db);
    qry.prepare("INSERT INTO matcher (name) VALUES (:name)");
    qry.bindValue(":name", name);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::delMatcher(const QString &name)
{
    QSqlQuery qry(m_db);
    qry.prepare("DELETE FROM matcher WHERE name = :name");
    qry.bindValue(":name", name);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::delMatcher(const matcherData_t &m_t)
{
    QSqlQuery qry(m_db);
    qry.prepare("DELETE FROM matcher WHERE name = :name");
    qry.bindValue(":name", m_t.name);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::retrieveToVec(std::vector<std::shared_ptr<matcherData_t>> &smart_vec)
{
    QSqlQuery qry(m_db);

    if ( !qry.exec("SELECT * FROM matcher") ) { return false; }

    while (qry.next())
    {

       auto id { qry.value("id").toString() };
       auto name { qry.value("name").toString() };
       auto p_t = std::make_shared<matcherData_t>(name);
       p_t.get()->id = id;
       smart_vec.push_back(p_t);
    }

    return true;
}

bool dbManager::addMailPass(const QString &name, const QString &mail, const QString &pass, const QString &hoster, const QString &proxyip, const QString &proxyport, const QString &useproxy)
{
    QSqlQuery qry(m_db);
    qry.prepare("INSERT INTO mailpass (name,mail,pass,hoster, proxyip, proxyport, useproxy) VALUES (:name,:mail,:pass,:hoster,:proxyip,:proxyport,:useproxy)");
    qry.bindValue(":name", name);
    qry.bindValue(":mail", mail);
    qry.bindValue(":pass", pass);
    qry.bindValue(":hoster", hoster);
    qry.bindValue(":proxyip", proxyip);
    qry.bindValue(":proxyport", proxyport);
    qry.bindValue(":useproxy", useproxy);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::updateMailPass(const QString &name, const QString &mail, const QString &pass, const QString &hoster, const QString &proxyip, const QString &proxyport, const QString &useproxy)
{
    QSqlQuery qry(m_db);
    qry.prepare("UPDATE mailpass SET name = :name, mail = :mail, pass = :pass, hoster = :hoster, proxyip = :proxyip, proxyport = :proxyport, useproxy = :useproxy WHERE name = :name");
    qry.bindValue(":name", name);
    qry.bindValue(":mail", mail);
    qry.bindValue(":pass", pass);
    qry.bindValue(":hoster", hoster);
    qry.bindValue(":proxyip", proxyip);
    qry.bindValue(":proxyport", proxyport);
    qry.bindValue(":useproxy", useproxy);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::delMailPass(const QString &name)
{
    QSqlQuery qry(m_db);
    qry.prepare("DELETE FROM mailpass WHERE name = :name");
    qry.bindValue(":name", name);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::delMailPass(const mailPass_t &mp_t)
{
    QSqlQuery qry(m_db);
    qry.prepare("DELETE FROM mailpass WHERE name = :name");
    qry.bindValue(":name", mp_t.name);
    if( !qry.exec() ) { return false; }
    return true;
}

mailPass_t dbManager::retrieveMailPassByName(const QString &name)
{
    QSqlQuery qry(m_db);

    mailPass_t mp_t;

    qry.prepare("SELECT * FROM mailpass WHERE name = :name");
    qry.bindValue(":name", name);
    if ( !qry.exec() ) { return mp_t; }

    qry.first();

    mp_t.name = qry.value("name").toString();
    mp_t.mail = qry.value("mail").toString();
    mp_t.pass = qry.value("pass").toString();
    mp_t.hoster = qry.value("hoster").toString();
    mp_t.proxyip = qry.value("proxyip").toString();
    mp_t.proxyport = qry.value("proxyport").toString();
    mp_t.useproxy = qry.value("useproxy").toString();

    return mp_t;
}

bool dbManager::retrieveToVec(std::vector<std::shared_ptr<mailPass_t>> &smart_vec)
{
    QSqlQuery qry(m_db);

    if ( !qry.exec("SELECT * FROM mailpass") ) { return false; }

    while (qry.next())
    {
        auto id { qry.value("id").toString() };
        auto name { qry.value("name").toString() };
        auto mail { qry.value("mail").toString() };
        auto pass { qry.value("pass").toString() };
        auto hoster { qry.value("hoster").toString() };
        auto proxyip { qry.value("proxyip").toString() };
        auto proxyport { qry.value("proxyport").toString() };
        auto useproxy { qry.value("useproxy").toString() };
        auto mp_t = std::make_shared<mailPass_t>(name, mail, pass, hoster, proxyip, proxyport, useproxy);
        mp_t.get()->id = id;
        smart_vec.push_back(mp_t);
    }

    return true;
}

bool dbManager::addHoster(const QString &name, const QString &addr, const QString &port)
{
    QSqlQuery qry(m_db);
    qry.prepare("INSERT INTO hoster (name,addr,port) VALUES (:name,:addr,:port)");
    qry.bindValue(":name", name);
    qry.bindValue(":addr", addr);
    qry.bindValue(":port", port);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::delHoster(const QString &name)
{
    QSqlQuery qry(m_db);
    qry.prepare("DELETE FROM hoster WHERE name = :name");
    qry.bindValue(":name", name);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::delHoster(const hoster_t &h_t)
{
    QSqlQuery qry(m_db);
    qry.prepare("DELETE FROM hoster WHERE name = :name");
    qry.bindValue(":name", h_t.name);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::retrieveToVec(std::vector<std::shared_ptr<hoster_t> > &smart_vec)
{
    QSqlQuery qry(m_db);

    if ( !qry.exec("SELECT * FROM hoster") ) { return false; }

    while (qry.next())
    {
        auto id { qry.value("id").toString() };
        auto name { qry.value("name").toString() };
        auto addr { qry.value("addr").toString() };
        auto port { qry.value("port").toString() };
        auto h_t = std::make_shared<hoster_t>(name, addr, port);
        h_t.get()->id = id;
        smart_vec.push_back(h_t);
    }

    return true;
}

bool dbManager::addMail(const QString &from, const QString &to, const QString &text)
{
    QSqlQuery qry(m_db);
    qry.prepare("INSERT INTO mail (froml,tol,textl) VALUES (:froml,:tol,:textl)");
    qry.bindValue(":froml", from);
    qry.bindValue(":tol", to);
    qry.bindValue(":textl", text);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::delMail(const QString &id)
{
    QSqlQuery qry(m_db);
    qry.prepare("DELETE FROM mail WHERE id = :id");
    qry.bindValue(":id", id);
    if( !qry.exec() ) { return false; }
    return true;
}

bool dbManager::delMail(const mailData_t &m_t)
{
    QSqlQuery qry(m_db);
    qry.prepare("DELETE FROM mail WHERE id = :id");
    qry.bindValue(":id", m_t.id);
    if( !qry.exec() ) { return false; }
    return true;
}

mailData_t dbManager::retrieveMailById(const QString &id)
{
    QSqlQuery qry(m_db);

    mailData_t m_t;

    qry.prepare("SELECT FROM mail WHERE id = :id");
    qry.bindValue(":id", id);
    if ( !qry.exec() ) { return m_t; }

    qry.first();

    m_t.from = qry.value("froml").toString();
    m_t.to = qry.value("tol").toString();
    m_t.text = qry.value("textl").toString();

    return m_t;
}

bool dbManager::retrieveToVec(std::vector<std::shared_ptr<mailData_t> > &smart_vec)
{
    QSqlQuery qry(m_db);

    if ( !qry.exec("SELECT * FROM mail") ) { return false; }

    while (qry.next())
    {
        auto id { qry.value("id").toString() };
        auto from { qry.value("froml").toString() };
        auto to { qry.value("tol").toString() };
        auto text { qry.value("textl").toString() };
        auto m_t = std::make_shared<mailData_t>(from, to, text);
        m_t.get()->id = id;
        smart_vec.push_back(m_t);
    }

    return true;
}
