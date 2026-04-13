#ifndef CPPFACTORY_H
#define CPPFACTORY_H

#include "factory.h"

/**
 * @brief Элемент "класс" на языке C++.
 */
class CppClassUnit : public ClassUnit
{
public:
    explicit CppClassUnit(const std::string& name, Flags flags = 0)
        : ClassUnit(name, flags)
    {
    }

    std::string compile(unsigned int level = 0) const override;
};

#endif
