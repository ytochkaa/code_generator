#ifndef METHODUNIT_H
#define METHODUNIT_H

#include "unit.h"

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

protected:
    std::string m_name;
    std::string m_returnType;
    Flags m_flags;
    std::vector<std::shared_ptr<Unit>> m_body;
};

#endif
