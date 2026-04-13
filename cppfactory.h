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

/**
 * @brief Элемент "функция-член" на языке C++.
 */
class CppMethodUnit : public MethodUnit
{
public:
    CppMethodUnit(const std::string& name, const std::string& returnType, Flags flags)
        : MethodUnit(name, returnType, flags)
    {
    }

    std::string compile(unsigned int level = 0) const override;
};

#endif
