#include "utils.hpp"

// check if given string is a numeric string or not
bool isNumber(const std::string& str)
{
    // std::find_first_not_of searches the string for the first character
    // that does not match any of the characters specified in its arguments
    return !str.empty() &&
        (str.find_first_not_of("[0123456789]") == std::string::npos);
}

// Function to split string str using given delimiter
std::vector<std::string> split(const std::string& str, char delim)
{
    auto i = 0;
    std::vector<std::string> list;

    auto pos = str.find(delim);

    while (pos != std::string::npos)
    {
        list.push_back(str.substr(i, pos - i));
        i = ++pos;
        pos = str.find(delim, pos);
    }

    list.push_back(str.substr(i, str.length()));

    return list;
}

// Function to validate an IP address
bool utils::validateIP(std::string ip)
{
    // split the string into tokens
    std::vector<std::string> list = split(ip, '.');

    // if token size is not equal to four
    if (list.size() != 4)
        return false;

    // validate each token
    for (std::string str : list)
    {
        // verify that string is number or not and the numbers
        // are in the valid range
        if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0)
            return false;
    }

    return true;
}

// Function to create a new directory
bool utils::createDir(const QString &path)
{
    if ( !QDir(path).exists() )
    {
        if ( !QDir().mkdir(path) ) { return false; }
    }

    return true;
}
