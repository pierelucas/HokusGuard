#ifndef HOKUSGUARDMAINWINDOW_HPP
#define HOKUSGUARDMAINWINDOW_HPP

// QT
#include <QMainWindow>
#include <QString>

// STL
#include <memory>
#include <vector>
#include <atomic>
#include <thread>

// Project
#include "database.hpp"
#include "mailData.hpp"
#include "hosterData.hpp"
#include "settingsData.hpp"
#include "addmaildialog.hpp"
#include "editmaildialog.hpp"
#include "mailopenwindow.hpp"
#include "optionwindow.hpp"

#define DEFAULT_THREADS 10

QT_BEGIN_NAMESPACE
namespace Ui { class HokusGuardMainWindow; }
QT_END_NAMESPACE

class HokusGuardMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    HokusGuardMainWindow(QWidget *parent = nullptr);
    ~HokusGuardMainWindow();

private:
    Ui::HokusGuardMainWindow *ui; 

    // Dialogs
    AddMailDialog *addmaildialog;
    EditMailDialog *editmaildialog;
    OptionWindow *optionwindow;

    // DB
    dbManager *db;

    // Thread Control
    std::atomic<bool> running;

    // Settings
    std::shared_ptr<settings_t> settingsData;

    // Vectors
    std::vector<std::shared_ptr<mailPass_t>> *mailPassVec;
    std::vector<std::shared_ptr<hoster_t>> *hosterVec;
    std::vector<std::shared_ptr<matcherData_t>> *matcherVec;
    std::vector<std::shared_ptr<mailData_t>> *mailDataVec;

    // Setup
    void setup();

    void startGuard();
    void stopGuard();

    void addMailToGuard();
    void editMailFromGuard();
    void deleteMailFromGuard();

    void showMailWindow();
    void showOptionWindow();
};
#endif // HOKUSGUARDMAINWINDOW_HPP
