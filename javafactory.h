#ifndef JAVAFACTORY_H
#define JAVAFACTORY_H

#include "factory.h"

/**
 * @brief Элемент "класс" на языке Java.
 */
class JavaClassUnit : public ClassUnit
{
public:
    explicit JavaClassUnit(const std::string& name, Flags flags = 0)
        : ClassUnit(name, flags)
    {
    }

    /**
     * @brief Возвращает ключевое слово доступа Java для модификатора.
     *
     * Для package-private доступа возвращает пустую строку.
     */
    static std::string accessModifierToString(int accessModifier);

    std::string compile(unsigned int level = 0) const override;
};

/**
 * @brief Элемент "функция-член" на языке Java.
 */
class JavaMethodUnit : public MethodUnit
{
public:
    JavaMethodUnit(const std::string& name, const std::string& returnType, Flags flags)
        : MethodUnit(name, returnType, flags)
    {
    }

    std::string compile(unsigned int level = 0) const override;
};

#endif
