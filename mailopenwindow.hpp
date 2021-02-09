#ifndef MAILOPENWINDOW_HPP
#define MAILOPENWINDOW_HPP

// QT
#include <QDialog>
#include <QKeyEvent>

// Project
#include "mailData.hpp"

namespace Ui {
class MailOpenWindow;
}

class MailOpenWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MailOpenWindow(QWidget *parent = nullptr);
    ~MailOpenWindow();

    void setMail(std::shared_ptr<mailData_t> m_t);
    std::shared_ptr<mailData_t> getMail() const;

    void setFROMText(const QString &from);
    void setTOText(const QString &to);
    void setMailText(const QString &text);
    void setDateText(const QString &date);

    bool getCanceled() const;

private:
    Ui::MailOpenWindow *ui;

    // Data
    std::shared_ptr<mailData_t> m_t;
    bool canceled;

    void accept() override;
    void reject() override;
    void keyPressEvent(QKeyEvent *e) override;

    // Open Sender Window
    void showSenderWindow();
};

// Inline functions

inline void MailOpenWindow::setMail(std::shared_ptr<mailData_t> m_t)
{
    this->m_t = m_t;
}

inline std::shared_ptr<mailData_t> MailOpenWindow::getMail() const
{
    return this->m_t;
}

inline bool MailOpenWindow::getCanceled() const
{
    return canceled;
}

#endif // MAILOPENWINDOW_HPP
