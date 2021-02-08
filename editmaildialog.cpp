#include "editmaildialog.hpp"
#include "ui_editmaildialog.h"

EditMailDialog::EditMailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditMailDialog)
{
    ui->setupUi(this);

    connect(this->ui->apply_pushButton, &QPushButton::clicked, this, &EditMailDialog::accept);
    connect(this->ui->reject_pushButton, &QPushButton::clicked, this, &EditMailDialog::reject);

    this->ui->email_lineEdit->setDisabled(true);
}

EditMailDialog::~EditMailDialog()
{
    delete ui;
}

void EditMailDialog::accept()
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
    this->mp_t.get()->proxyip = proxyIP;
    this->mp_t.get()->proxyport = proxyPORT;
    this->mp_t.get()->hosterIMAPDomain = IMAPDomain;
    this->mp_t.get()->hosterIMAPPort = IMAPPort;

    this->mp_t->useproxy = (this->ui->use_proxy_radioButton->isChecked()) ? "1" : "0";

    this->canceled = false;
    QDialog::accept();
}

void EditMailDialog::reject()
{
    this->canceled = true;
    QDialog::reject();
}

void EditMailDialog::keyPressEvent(QKeyEvent *e)
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

void EditMailDialog::setMailText(const QString &mail)
{
    this->ui->email_lineEdit->setText(mail);
    QWidget::setWindowTitle("Edit Mail @ " + mail);
}

void EditMailDialog::setPasswordText(const QString &pass)
{
    this->ui->password_lineEdit->setText(pass);
}

void EditMailDialog::setProxyIPText(const QString &proxyip)
{
    this->ui->proxy_ip_lineEdit->setText(proxyip);
}

void EditMailDialog::setProxyPORTText(const QString &proxyport)
{
    this->ui->proxy_port_lineEdit->setText(proxyport);
}

void EditMailDialog::setProxyState(const QString &useproxy)
{
    if ( useproxy.toInt() > 0 )
    {
        this->ui->use_proxy_radioButton->setChecked(true);
    }
    else
    {
        this->ui->use_proxy_radioButton->setChecked(false);

    }
}

void EditMailDialog::setIMAPDomainText(const QString &imapdomain)
{
    this->ui->imap_domain_lineEdit->setText(imapdomain);
}

void EditMailDialog::setIMAPPortText(const QString &imapport)
{
    this->ui->imap_port_lineEdit->setText(imapport);
}
