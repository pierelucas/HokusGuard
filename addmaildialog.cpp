#include "addmaildialog.hpp"
#include "ui_addmaildialog.h"

AddMailDialog::AddMailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMailDialog)
{
    ui->setupUi(this);

    connect(this->ui->apply_pushButton, &QPushButton::clicked, this, &AddMailDialog::saveAccept);
    connect(this->ui->reject_pushButton, &QPushButton::clicked, this, &AddMailDialog::reject);
}

AddMailDialog::~AddMailDialog()
{
    delete ui;
}

void AddMailDialog::saveAccept()
{
    auto mail = this->ui->email_lineEdit->text();
    auto pwd = this->ui->password_lineEdit->text();
    auto proxyIP = this->ui->proxy_ip_lineEdit->text();
    auto proxyPORT = this->ui->proxy_port_lineEdit->text();

    // Validate Mail
    mail = mail.simplified();
    mail.replace(" ", "");
    auto dotcheck { mail.split("@") };
    if ( dotcheck.size() != 2 )
    {
        QMessageBox::information(this, "Mail Error", "this is not a valid mail address: " + mail);
        return;
    }
    if ( dotcheck[1].split(".").size() != 2 )
    {
        QMessageBox::information(this, "Service Error", "this is not a valid service: " + dotcheck[1]);
        return;
    }

    auto hoster { dotcheck[1] };

    // Validate Proxy
    // Sanitize the string - cut off whitespaces,\r,\n,\t
    proxyIP = proxyIP.simplified();
    proxyIP.replace(" ", "");
    proxyPORT = proxyPORT.simplified();
    proxyPORT.replace(" ", "");

    if ( proxyIP.size() <= 0 )
    {
        QMessageBox::information(this, "IP Error", "ip is empty");
        return;
    }
    if ( !utils::validateIP(proxyIP.toUtf8().toStdString()) )
    {
        QMessageBox::information(this, "IP Error", "not a valid IPV4 address");
        return;
    }
    if ( proxyPORT <= 0 )
    {
        QMessageBox::information(this, "Port Error", "port is empty");
        return;
    }
    if ( proxyPORT.toUInt() <= 0 || proxyPORT.toUInt() > 65535 )
    {
        QMessageBox::information(this, "Port Error", "not a valid port");
        return;
    }

    this->mp_t.get()->name = mail;
    this->mp_t.get()->mail = mail;
    this->mp_t.get()->pass = pwd;
    this->mp_t.get()->hoster = hoster;
    this->mp_t.get()->proxyip = proxyIP;
    this->mp_t.get()->proxyport = proxyPORT;

    this->accept();
}
