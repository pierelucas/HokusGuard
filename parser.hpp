#ifndef PARSER_HPP
#define PARSER_HPP

// QT
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QList>

// STL
#include <memory>

// Project
#include "hosterData.hpp"

namespace Parser
{
    bool hosterfile(const QString &path, std::vector<std::shared_ptr<hoster_t>> &vec);
}

#endif // PARSER_HPP
