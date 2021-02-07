#ifndef SETTINGSDATA_HPP
#define SETTINGSDATA_HPP

// QT
#include <QString>

// STL
#include <cstdint>

struct settings_t final
{
    uint32_t cpuCount;
    QString backupBaseDir;
    QString backupDir;
};

#endif // SETTINGSDATA_HPP
