#ifndef CSHARPFACTORY_H
#define CSHARPFACTORY_H

#include "factory.h"

/**
 * @brief Элемент "класс" на языке C#.
 */
class CSharpClassUnit : public ClassUnit
{
public:
    explicit CSharpClassUnit(const std::string& name, Flags flags = 0)
        : ClassUnit(name, flags)
    {
    }

    /**
     * @brief Возвращает ключевое слово доступа C# для модификатора.
     */
    static std::string accessModifierToString(int accessModifier);

    std::string compile(unsigned int level = 0) const override;
};

/**
 * @brief Элемент "функция-член" на языке C#.
 */
class CSharpMethodUnit : public MethodUnit
{
public:
    CSharpMethodUnit(const std::string& name, const std::string& returnType, Flags flags)
        : MethodUnit(name, returnType, flags)
    {
    }

    std::string compile(unsigned int level = 0) const override;
};

#endif
