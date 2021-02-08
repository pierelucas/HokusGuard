#include "addmaildialog.hpp"
#include "ui_addmaildialog.h"

AddMailDialog::AddMailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMailDialog)
{
    ui->setupUi(this);

    connect(this->ui->apply_pushButton, &QPushButton::clicked, this, &AddMailDialog::accept);
    connect(this->ui->reject_pushButton, &QPushButton::clicked, this, &AddMailDialog::reject);
    connect(this->ui->imap_autofill_pushButton, &QPushButton::clicked, this, &AddMailDialog::autoFillIMAP);
    connect(this->ui->imap_autofill_add_pushButton, &QPushButton::clicked, this, &AddMailDialog::addHosterToDatabase);
}

AddMailDialog::~AddMailDialog()
{
    delete ui;
}

void AddMailDialog::accept()
{
    auto mail = this->ui->email_lineEdit->text();
    auto pwd = this->ui->password_lineEdit->text();
    auto proxyIP = this->ui->proxy_ip_lineEdit->text();
    auto proxyPORT = this->ui->proxy_port_lineEdit->text();
    auto IMAPDomain = this->ui->imap_domain_lineEdit->text();
    auto IMAPPort = this->ui->imap_port_lineEdit->text();

    // Validate Mail
    mail = mail.simplified();
    mail.replace(" ", "");
    auto dotcheck { mail.split("@") };
    if ( dotcheck.size() != 2 )
    {
        auto prtStr = (mail == "") ? "<empty>" : mail;
        QMessageBox::information(this, "Mail Error", "this is not a valid mail address: " + prtStr);
        return;
    }
    if ( dotcheck[1].split(".").size() != 2 )
    {
        QMessageBox::information(this, "Service Error", "this is not a valid service: " + dotcheck[1]);
        return;
    }

    // Validate Password
    pwd = pwd.simplified();
    pwd.replace(" ", "");
    if ( pwd.size() < 1 )
    {
        QMessageBox::information(this, "Password Error", "password can't be empty");
        return;
    }

    auto hoster { dotcheck[1] };

    // Validate Proxy when proxy is set
    // Sanitize the string - cut off whitespaces,\r,\n,\t
    proxyIP = proxyIP.simplified();
    proxyIP.replace(" ", "");
    proxyPORT = proxyPORT.simplified();
    proxyPORT.replace(" ", "");
    if ( this->ui->use_proxy_radioButton->isChecked() ||
         proxyIP.size() > 0 ||
         proxyPORT.size() > 0 )
    {
        if ( proxyIP.size() <= 1 )
        {
            QMessageBox::information(this, "IP Error", "ip is empty");
            return;
        }
        if ( !utils::validateIP(proxyIP.toUtf8().toStdString()) )
        {
            QMessageBox::information(this, "IP Error", "not a valid IPV4 address");
            return;
        }
        if ( proxyPORT.size() <= 1 )
        {
            QMessageBox::information(this, "Proxy Port Error", "port is empty");
            return;
        }
        if ( proxyPORT.toUInt() <= 0 || proxyPORT.toUInt() > 65535 )
        {
            QMessageBox::information(this, "Proxy Port Error", "not a valid port");
            return;
        }

        this->mp_t.get()->proxyip = proxyIP;
        this->mp_t.get()->proxyport = proxyPORT;
    }
    else
    {
        this->mp_t.get()->proxyip = "";
        this->mp_t.get()->proxyport = "";
    }

    // Validate IMAP
    IMAPDomain = IMAPDomain.simplified();
    IMAPDomain.replace(" ", "");
    IMAPPort = IMAPPort.simplified();
    IMAPPort.replace(" ", "");
    if ( IMAPDomain.size() <= 1 )
    {
        QMessageBox::information(this, "Domain Error", "IMAP Domain is empty");
        return;
    }
    if ( IMAPPort.size() <= 1 )
    {
        QMessageBox::information(this, "IMAP Port Error", "port is empty");
        return;
    }
    if ( IMAPPort.toUInt() <= 0 || IMAPPort.toUInt() > 65535 )
    {
        QMessageBox::information(this, "IMAP Port Error", "not a valid port");
        return;
    }

    this->mp_t.get()->name = mail;
    this->mp_t.get()->mail = mail;
    this->mp_t.get()->pass = pwd;
    this->mp_t.get()->hoster = hoster;
    this->mp_t.get()->hosterIMAPDomain = IMAPDomain;
    this->mp_t.get()->hosterIMAPPort = IMAPPort;

    this->mp_t->useproxy = (this->ui->use_proxy_radioButton->isChecked()) ? "1" : "0";

    this->canceled = false;
    QDialog::accept();
}

void AddMailDialog::reject()
{
    this->canceled = true;
    QDialog::reject();
}

void AddMailDialog::keyPressEvent(QKeyEvent *e)
{
    switch ( e->key() )
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        this->accept();
        break;
    case Qt::Key_Escape:
        this->reject();
        break;
    default:
        break;
    }
}

void AddMailDialog::resetUI()
{
    this->ui->email_lineEdit->setText("");
    this->ui->password_lineEdit->setText("");
    this->ui->proxy_ip_lineEdit->setText("");
    this->ui->proxy_port_lineEdit->setText("");
    this->ui->use_proxy_radioButton->setChecked(false);
    this->ui->imap_domain_lineEdit->setText("");
    this->ui->imap_port_lineEdit->setText("");
}

void AddMailDialog::autoFillIMAP()
{
    auto mail = this->ui->email_lineEdit->text();

    // Validate Mail
    mail = mail.simplified();
    mail.replace(" ", "");
    auto dotcheck { mail.split("@") };
    if ( dotcheck.size() != 2 )
    {
        auto prtStr = (mail == "") ? "<empty>" : mail;
        QMessageBox::information(this, "Mail Error", "this is not a valid mail address: " + prtStr);
        return;
    }
    if ( dotcheck[1].split(".").size() != 2 )
    {
        QMessageBox::information(this, "Service Error", "this is not a valid service: " + dotcheck[1]);
        return;
    }

    auto hoster { dotcheck[1] };

    // Get hosterdata from db
    auto h_t = db->retrieveHosterByName(hoster);

    if ( h_t.name.size() < 1 )
    {
        QMessageBox::information(this, "No Data", "There's no entry for: " + hoster);
        return;
    }

    this->ui->imap_domain_lineEdit->setText(h_t.addr);
    this->ui->imap_port_lineEdit->setText(h_t.port);

    return;
}

void AddMailDialog::addHosterToDatabase()
{
    QString path { QFileDialog::getOpenFileName(this, "Select hoster to open", QDir::homePath()) };

    // Return if the user close the file dialog without choosing a valid path
    if ( path.size() <= 0 ) { return; }

    auto temp_vec = new std::vector<std::shared_ptr<hoster_t>>;
    if ( !Parser::hosterfile(path, *temp_vec) )
    {
        QMessageBox::critical(this, "Hoster Error", path + " is not valid or unreadable");
        return;
    }

    for ( const auto h_v : *temp_vec )
    {
        if ( !db->addHoster(h_v.get()->name, h_v.get()->addr, h_v.get()->port) )
        {
            // TODO: Find a better solution
            //QMessageBox::critical(this, "DB Error", "internal database error: write()");
            //return;
        }
    }

}
