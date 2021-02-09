#ifndef OPTIONWINDOW_HPP
#define OPTIONWINDOW_HPP

// QT
#include <QDialog>
#include <QKeyEvent>

// Project
#include "database.hpp"

namespace Ui {
class OptionWindow;
}

class OptionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OptionWindow(QWidget *parent = nullptr);
    ~OptionWindow();

    void setDB(dbManager *db);

    void setup();

    bool getCanceled() const;

private:
    Ui::OptionWindow *ui;

    // DB
    dbManager *db;

    // Data
    bool canceled;

    void accept() override;
    void reject() override;
    void keyPressEvent(QKeyEvent *e) override;
};

// Inline functions

inline void OptionWindow::setDB(dbManager *db)
{
    this->db = db;
}

inline bool OptionWindow::getCanceled() const
{
    return canceled;
}

#endif // OPTIONWINDOW_HPP
