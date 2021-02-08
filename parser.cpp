#include "parser.hpp"

bool Parser::hosterfile(const QString &path, std::vector<std::shared_ptr<hoster_t>> &vec)
{
    QFile inputFile(path);
    if ( !inputFile.open(QIODevice::ReadOnly) ) { return false; }

    ulong counter { 0UL };

    QTextStream in(&inputFile);
    while (!in.atEnd())
    {
       QString line = in.readLine();
       QStringList list { line.split(":") };

       // Check for valid mail
       if ( list.size() != 3 ) { continue; }

       // Increase counter if we found a possible valid entrie
       counter++;

       auto name { list[0] };
       name = name.simplified();
       name.replace(" ", "");

       auto addr { list[1] };
       addr = addr.simplified();
       addr.replace(" ", "");

       auto port { list[2] };
       port = port.simplified();
       port.replace(" ", "");

       auto h_v = std::make_shared<hoster_t>(name, addr, port);
       vec.push_back(h_v);
    }
    inputFile.close();

    if ( counter == 0 ) { return false; }

    return true;
}
