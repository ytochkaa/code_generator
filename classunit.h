#ifndef CLASSUNIT_H
#define CLASSUNIT_H

#include "unit.h"

#include <map>
#include <vector>

/**
 * @brief Абстрактный элемент "класс".
 *
 * Хранит вложенные элементы, сгруппированные по модификатору доступа.
 * Конкретную генерацию кода выполняют наследники для каждого языка.
 */
class ClassUnit : public Unit
{
public:
    /**
     * @brief Модификаторы доступа всех поддерживаемых языков.
     */
    enum AccessModifier
    {
        PUBLIC,
        PROTECTED,
        PRIVATE,
        INTERNAL,
        PROTECTED_INTERNAL,
        PRIVATE_PROTECTED,
        PACKAGE_PRIVATE
    };

    /**
     * @brief Модификаторы уровня класса (битовые флаги).
     */
    enum Modifier
    {
        STATIC = 1,
        ABSTRACT = 1 << 1,
        FINAL = 1 << 2,
        PARTIAL = 1 << 3
    };

    explicit ClassUnit(const std::string& name, Flags flags = 0)
        : m_name(name)
        , m_flags(flags)
    {
    }

    /**
     * @brief Задаёт базовый класс (наследование).
     */
    void setParent(const std::string& parent) { m_parent = parent; }

    /**
     * @brief Добавляет элемент в нужную секцию доступа.
     * @param unit  вложенный элемент.
     * @param flags модификатор доступа.
     */
    void add(const std::shared_ptr<Unit>& unit, Flags flags) override
    {
        int accessModifier = PRIVATE;
        if (flags <= PACKAGE_PRIVATE) {
            accessModifier = flags;
        }
        unit->setAccessModifier(accessModifier);
        m_fields[accessModifier].push_back(unit);
    }

protected:
    std::string m_name;
    Flags m_flags;
    std::string m_parent;
    std::map<int, std::vector<std::shared_ptr<Unit>>> m_fields;
};

#endif
