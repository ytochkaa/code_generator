#ifndef METHODUNIT_H
#define METHODUNIT_H

#include "unit.h"

#include <stdexcept>
#include <vector>

/**
 * @brief Абстрактный элемент "функция-член".
 *
 * Хранит имя, тип возвращаемого значения, набор модификаторов и тело.
 * Генерацию кода выполняют наследники для каждого языка.
 */
class MethodUnit : public Unit
{
public:
    /**
     * @brief Модификаторы метода всех поддерживаемых языков (битовые флаги).
     */
    enum Modifier
    {
        STATIC = 1,
        CONST = 1 << 1,
        VIRTUAL = 1 << 2,
        ABSTRACT = 1 << 3,
        OVERRIDE = 1 << 4,
        SEALED = 1 << 5,
        FINAL = 1 << 6,
        ASYNC = 1 << 7
    };

    MethodUnit(const std::string& name, const std::string& returnType, Flags flags)
        : m_name(name)
        , m_returnType(returnType)
        , m_flags(flags)
    {
    }

    /**
     * @brief Добавляет элемент в тело метода.
     */
    void add(const std::shared_ptr<Unit>& unit, Flags = 0) override
    {
        m_body.push_back(unit);
    }

    const std::string& name() const { return m_name; }
    Flags flags() const { return m_flags; }

protected:
    /**
     * @brief Проверяет сочетания модификаторов, недопустимые во всех языках.
     * @param language название языка для текста ошибки.
     * @throw std::logic_error при недопустимом сочетании.
     */
    void checkCommonFlags(const std::string& language) const
    {
        if ((m_flags & STATIC) && (m_flags & (VIRTUAL | ABSTRACT | OVERRIDE | SEALED))) {
            fail(language, "static нельзя сочетать с virtual, abstract, override или sealed");
        }
        if ((m_flags & ABSTRACT) && (m_flags & (SEALED | FINAL))) {
            fail(language, "abstract нельзя сочетать с sealed или final");
        }
        if ((m_flags & SEALED) && !(m_flags & OVERRIDE)) {
            fail(language, "sealed допустим только вместе с override");
        }
    }

    /**
     * @brief Выбрасывает исключение с описанием ошибки в методе.
     */
    [[noreturn]] void fail(const std::string& language, const std::string& message) const
    {
        throw std::logic_error(language + ": метод " + m_name + ": " + message);
    }

    /**
     * @brief Генерирует тело метода в фигурных скобках.
     */
    std::string compileBody(unsigned int level) const
    {
        std::string result = " {\n";
        for (const auto& statement : m_body) {
            result += statement->compile(level + 1);
        }
        result += generateShift(level) + "}\n";
        return result;
    }

    std::string m_name;
    std::string m_returnType;
    Flags m_flags;
    std::vector<std::shared_ptr<Unit>> m_body;
};

#endif
