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

/**
 * @brief Элемент "оператор печати" на языке C#.
 */
class CSharpPrintOperatorUnit : public PrintOperatorUnit
{
public:
    explicit CSharpPrintOperatorUnit(const std::string& text)
        : PrintOperatorUnit(text)
    {
    }

    std::string compile(unsigned int level = 0) const override;
};

/**
 * @brief Фабрика элементов языка C#.
 */
class CSharpFactory : public Factory
{
public:
    std::shared_ptr<ClassUnit> createClass(const std::string& name, Unit::Flags flags = 0) const override;
    std::shared_ptr<MethodUnit> createMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) const override;
    std::shared_ptr<PrintOperatorUnit> createPrintOperator(const std::string& text) const override;
    std::string wrapProgram(const std::string& classCode) const override;
    std::string fileName() const override;
};

#endif
