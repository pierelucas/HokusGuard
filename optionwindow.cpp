#include "optionwindow.hpp"
#include "ui_optionwindow.h"

OptionWindow::OptionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionWindow)
{
    ui->setupUi(this);

    connect(this->ui->apply_pushButton, &QPushButton::clicked, this, &OptionWindow::accept);
    connect(this->ui->reject_pushButton, &QPushButton::clicked, this, &OptionWindow::reject);
}

OptionWindow::~OptionWindow()
{
    delete ui;
}

void OptionWindow::setup()
{
    // TODO: retrieve option keys|values from databse
}

void OptionWindow::accept()
{
    this->canceled = false;
    QDialog::accept();
}

void OptionWindow::reject()
{
    this->canceled = true;
    QDialog::reject();
}

void OptionWindow::keyPressEvent(QKeyEvent *e)
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
