#ifndef ADDMAILDIALOG_HPP
#define ADDMAILDIALOG_HPP

// QT
#include <QDialog>
#include <QStringList>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFileDialog>

// STL
#include <memory>
#include <iostream>

// Project
#include "database.hpp"
#include "mailData.hpp"
#include "utils.hpp"
#include "parser.hpp"

namespace Ui {
class AddMailDialog;
}

class AddMailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMailDialog(QWidget *parent = nullptr);
    ~AddMailDialog();

    void setMail(std::shared_ptr<mailPass_t> mp_t);
    std::shared_ptr<mailPass_t> getMail() const;

    void setDB(dbManager *db);

    void resetUI();

    bool getCanceled() const;

private:
    Ui::AddMailDialog *ui;

    // DB
    dbManager *db;

    // Data
    std::shared_ptr<mailPass_t> mp_t;
    bool canceled;

    void accept() override;
    void reject() override;
    void keyPressEvent(QKeyEvent *e) override;

    void autoFillIMAP();

    // Add to database
    void addHosterToDatabase();
};

// Inline functions

inline void AddMailDialog::setMail(std::shared_ptr<mailPass_t> mp_t)
{
    this->mp_t = mp_t;
}

inline std::shared_ptr<mailPass_t> AddMailDialog::getMail() const
{
    return this->mp_t;
}

inline void AddMailDialog::setDB(dbManager *db)
{
    this->db = db;
}

inline bool AddMailDialog::getCanceled() const
{
    return canceled;
}

#endif // ADDMAILDIALOG_HPP
