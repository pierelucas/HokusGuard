#include "mailopenwindow.hpp"
#include "ui_mailopenwindow.h"

MailOpenWindow::MailOpenWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MailOpenWindow)
{
    ui->setupUi(this);
}

MailOpenWindow::~MailOpenWindow()
{
    delete ui;
}

void MailOpenWindow::setFROMText(const QString &from)
{
    this->ui->from_text_label->setText(from);
}

void MailOpenWindow::setTOText(const QString &to)
{
    this->ui->to_text_label->setText(to);
}

void MailOpenWindow::setMailText(const QString &text)
{
    this->ui->read_textBrowser->setText(text);
}

void MailOpenWindow::setDateText(const QString &date)
{
    this->ui->date_text_label->setText(date);
}

void MailOpenWindow::accept()
{
    this->canceled = false;
    QDialog::accept();
}

void MailOpenWindow::reject()
{
    this->canceled = true;
    QDialog::reject();
}

void MailOpenWindow::keyPressEvent(QKeyEvent *e)
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

void MailOpenWindow::showSenderWindow()
{

}

