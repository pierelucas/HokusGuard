#include "hokusguardmainwindow.hpp"
#include "ui_hokusguardmainwindow.h"

HokusGuardMainWindow::HokusGuardMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HokusGuardMainWindow)
    , addmaildialog(new AddMailDialog)
    , editmaildialog(new EditMailDialog)
    , optionwindow(new OptionWindow)
    , db(new dbManager)
    , running(false)
    , settingsData(std::make_shared<settings_t>())
    , mailPassVec(new std::vector<std::shared_ptr<mailPass_t>>)
    , hosterVec(new std::vector<std::shared_ptr<hoster_t>>)
    , matcherVec(new std::vector<std::shared_ptr<matcherData_t>>)
    , mailDataVec(new std::vector<std::shared_ptr<mailData_t>>)
{
    ui->setupUi(this);

    connect(this->ui->addMailPass_pushButton, &QPushButton::clicked, this, &HokusGuardMainWindow::addMailToGuard);
    connect(this->ui->mailpass_listWidget, &QListWidget::itemDoubleClicked, this, &HokusGuardMainWindow::editMailFromGuard);
    connect(this->ui->deleteMailPass_pushButton, &QPushButton::clicked, this, &HokusGuardMainWindow::deleteMailFromGuard);
    connect(this->ui->startGuard_pushButton, &QPushButton::clicked, this, &HokusGuardMainWindow::startGuard);
    connect(this->ui->stopGuard_pushButton, &QPushButton::clicked, this, &HokusGuardMainWindow::stopGuard);
    connect(this->ui->mail_listWidget, &QListWidget::itemDoubleClicked, this, &HokusGuardMainWindow::showMailWindow);
    connect(this->ui->settings_pushButton, &QPushButton::clicked, this, &HokusGuardMainWindow::showOptionWindow);

    this->setup();
}

HokusGuardMainWindow::~HokusGuardMainWindow()
{
    this->mailPassVec->clear();
    this->mailPassVec->shrink_to_fit();
    this->hosterVec->clear();
    this->hosterVec->shrink_to_fit();
    this->matcherVec->clear();
    this->matcherVec->shrink_to_fit();
    this->mailDataVec->clear();
    this->mailDataVec->shrink_to_fit();

    delete ui;
    delete addmaildialog;
    delete editmaildialog;
    delete optionwindow;
    delete db;
    delete mailPassVec;
    delete hosterVec;
    delete matcherVec;
    delete mailDataVec;
}

void HokusGuardMainWindow::setup()
{
    // Get CPU Count
        auto cpuCount = std::thread::hardware_concurrency();
        this->settingsData.get()->cpuCount = (cpuCount != 0) ? cpuCount : DEFAULT_THREADS;

    // Setup MailPass list widget
        this->mailPassVec->clear();
        this->mailPassVec->shrink_to_fit();

        // Select multiple lines
        //this->ui->mailpass_listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

        if ( !(db->retrieveToVec(*mailPassVec)) )
        {
            QMessageBox::critical(this, "DB Error", "internal database error: read()");
            return;
        }

        this->ui->mailpass_listWidget->clear();
        for ( const auto mp_v : *mailPassVec )
        {
            this->ui->mailpass_listWidget->addItem(mp_v.get()->mail);
        }

    // Setup Mail list widget
        this->mailDataVec->clear();
        this->mailDataVec->shrink_to_fit();

        if ( !(db->retrieveToVec(*mailDataVec)) )
        {
            QMessageBox::critical(this, "DB Error", "internal database error: read()");
            return;
        }

        this->ui->mail_listWidget->clear();
        for ( const auto m_v : *mailDataVec )
        {
            QString textSlice;
            for ( int i = 0; i < 50; i++ )
            {
                if ( i == m_v.get()->text.length() ) { break; }
                textSlice += m_v.get()->text.at(i);
            }
            QString text = m_v.get()->id + " | " + "FROM: " + m_v.get()->from + " TO: " + m_v.get()->to + " TEXT: " + textSlice + "...";
            this->ui->mail_listWidget->addItem(text);
        }
}

void HokusGuardMainWindow::startGuard()
{
    // TODO: Implement
}

void HokusGuardMainWindow::stopGuard()
{
    // TODO: Implement
}

void HokusGuardMainWindow::addMailToGuard()
{
    this->ui->addMailPass_pushButton->setDisabled(true);
    this->ui->addMailPass_pushButton->setStyleSheet(
    "QPushButton\
    {\
        border-radius:15px;\
        background-color:grey;\
        color:white;\
    }\
    QPushButton:hover\
    {\
        border-radius:15px;\
        background-color:#feb83d;\
        color:black;\
    }");

    auto mp_t { std::make_shared<mailPass_t>() };
    addmaildialog->setMail(mp_t);
    addmaildialog->setDB(db);

    addmaildialog->show();
    addmaildialog->exec();

    this->ui->addMailPass_pushButton->setDisabled(false);
    this->ui->addMailPass_pushButton->setStyleSheet(
    "QPushButton\
    {\
        border-radius:15px;\
        background-color:#fa2569;\
        color:white;\
    }\
    QPushButton:hover\
    {\
        border-radius:15px;\
        background-color:#feb83d;\
        color:black;\
    }");

    if ( addmaildialog->getCanceled() ) { return; }

    if ( !db->addMailPass(mp_t.get()->name, mp_t.get()->mail, mp_t.get()->pass, mp_t.get()->hoster, mp_t.get()->hosterIMAPDomain, mp_t.get()->hosterIMAPPort, mp_t.get()->proxyip, mp_t.get()->proxyport, mp_t.get()->useproxy) )
    {
        // TODO: Find a better solution
        //QMessageBox::critical(this, "DB Error", "internal database error: write()");
        //return;
    }

    mailPassVec->clear();
    mailPassVec->shrink_to_fit();
    if ( !(db->retrieveToVec(*mailPassVec)) )
    {
        QMessageBox::critical(this, "DB Error", "internal database error: read()");
        return;
    }

    this->ui->mailpass_listWidget->clear();
    for ( const auto mp_v : *mailPassVec )
    {
        this->ui->mailpass_listWidget->addItem(mp_v.get()->mail);
    }

    // Reset addmaildialog ui
    addmaildialog->resetUI();
}

void HokusGuardMainWindow::editMailFromGuard()
{
    QList<QListWidgetItem*> items = this->ui->mailpass_listWidget->selectedItems();

    auto tmp { db->retrieveMailPassByName(items[0]->text()) };
    auto mp_t { std::make_shared<mailPass_t>(tmp) };

    editmaildialog->setMail(mp_t);

    editmaildialog->show();

    editmaildialog->setMailText(tmp.mail);
    editmaildialog->setPasswordText(tmp.pass);
    editmaildialog->setProxyIPText(tmp.proxyip);
    editmaildialog->setProxyPORTText(tmp.proxyport);
    editmaildialog->setProxyState(tmp.useproxy);
    editmaildialog->setIMAPDomainText(tmp.hosterIMAPDomain);
    editmaildialog->setIMAPPortText(tmp.hosterIMAPPort);

    editmaildialog->exec();

    if ( editmaildialog->getCanceled() ) { return; }

    // TODO: Add more checks when we have more values
    if ( tmp.pass == mp_t.get()->pass &&
         tmp.proxyip == mp_t.get()->proxyip &&
         tmp.proxyport == mp_t.get()->proxyport &&
         tmp.useproxy == mp_t.get()->useproxy &&
         tmp.hosterIMAPDomain == mp_t.get()->hosterIMAPDomain &&
         tmp.hosterIMAPPort == mp_t.get()->hosterIMAPPort )
    { return; }

    if ( !db->updateMailPass(mp_t.get()->name, mp_t.get()->mail, mp_t.get()->pass, mp_t.get()->hoster, mp_t.get()->hosterIMAPDomain, mp_t.get()->hosterIMAPPort, mp_t.get()->proxyip, mp_t->proxyport, mp_t.get()->useproxy) )
    {
        // TODO: Find a better solution
        //QMessageBox::critical(this, "DB Error", "internal database error: write()");
        //return;
    }

    mailPassVec->clear();
    mailPassVec->shrink_to_fit();
    if ( !(db->retrieveToVec(*mailPassVec)) )
    {
        QMessageBox::critical(this, "DB Error", "internal database error: read()");
        return;
    }

    this->ui->mailpass_listWidget->clear();
    for ( const auto mp_v : *mailPassVec )
    {
        this->ui->mailpass_listWidget->addItem(mp_v.get()->mail);
    }
}

void HokusGuardMainWindow::deleteMailFromGuard()
{
    QList<QListWidgetItem*> items = this->ui->mailpass_listWidget->selectedItems();
    if ( items.size() <= 0 )
    {
        QMessageBox::information(this, "Mail Error", "please select mail");
        return;
    }

    if( !(db->delMailPass(items[0]->text())) )
    {
        QMessageBox::critical(this, "DB Erro", "internal database error: delete()");
        return;
    }

    mailPassVec->clear();
    mailPassVec->shrink_to_fit();
    if ( !(db->retrieveToVec(*mailPassVec)) )
    {
        QMessageBox::critical(this, "DB Error", "internal database error: read()");
        return;
    }

    this->ui->mailpass_listWidget->clear();
    for ( const auto mp_v : *mailPassVec )
    {
        this->ui->mailpass_listWidget->addItem(mp_v.get()->mail);
    }
}

void HokusGuardMainWindow::showMailWindow()
{
    auto mailopenwindow = std::make_shared<MailOpenWindow>();

    QList<QListWidgetItem*> items = this->ui->mail_listWidget->selectedItems();

    auto id = items[0]->text().split("|")[0].simplified().replace(" ", "");
    auto tmp { db->retrieveMailById(id) };
    auto m_t { std::make_shared<mailData_t>(tmp) };

    mailopenwindow->setMail(m_t);

    mailopenwindow.get()->show();

    mailopenwindow->setFROMText(tmp.from);
    mailopenwindow->setTOText(tmp.to);
    mailopenwindow->setMailText(tmp.text);
    mailopenwindow->setDateText(tmp.date);

    mailopenwindow.get()->exec();
}

void HokusGuardMainWindow::showOptionWindow()
{
    optionwindow->setDB(db);

    optionwindow->show();

    optionwindow->setup();

    optionwindow->exec();
}
