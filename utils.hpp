#ifndef UTILS_HPP
#define UTILS_HPP

// QT
#include <QString>
#include <QDir>

// STL
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iterator>

namespace utils
{
    bool validateIP(std::string ip);

    bool createDir(const QString &path);

    template<typename T>
    T random(std::vector<T> const &v)
    {
        auto it = v.cbegin();
        int random = rand() % v.size();

        return *(it + random);
    }

    template<typename Iter, typename RandomGenerator>
    Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(g));
        return start;
    }

    template<typename Iter>
    Iter select_randomly(Iter start, Iter end) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return select_randomly(start, end, gen);
    }
}

#endif // UTILS_HPP
