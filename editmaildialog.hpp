#ifndef EDITMAILDIALOG_HPP
#define EDITMAILDIALOG_HPP

#include <QDialog>

// QT
#include <QDialog>
#include <QStringList>
#include <QMessageBox>

// STL
#include <memory>
#include <iostream>

// Project
#include "mailData.hpp"
#include "utils.hpp"

namespace Ui {
class EditMailDialog;
}

class EditMailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditMailDialog(QWidget *parent = nullptr);
    ~EditMailDialog();

    void setMail(std::shared_ptr<mailPass_t> mp_t);
    std::shared_ptr<mailPass_t> getMail() const;

    void setMailText(const QString &mail);
    void setPasswordText(const QString &pass);
    void setProxyIPText(const QString &proxyip);
    void setProxyPORTText(const QString &proxyport);
    void setProxyState(const QString &useproxy);
    void setIMAPDomainText(const QString &imapdomain);
    void setIMAPPortText(const QString &imapport);

    bool getCanceled() const;

private:
    Ui::EditMailDialog *ui;

    // Data
    std::shared_ptr<mailPass_t> mp_t;
    bool canceled;

    void accept() override;
    void reject() override;
};

// Inline functions

inline void EditMailDialog::setMail(std::shared_ptr<mailPass_t> mp_t)
{
    this->mp_t = mp_t;
}

inline std::shared_ptr<mailPass_t> EditMailDialog::getMail() const
{
    return this->mp_t;
}

inline bool EditMailDialog::getCanceled() const
{
    return canceled;
}

#endif // EDITMAILDIALOG_HPP
