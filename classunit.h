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
        PRIVATE_PROTECTED
    };

    /**
     * @brief Модификаторы класса (битовые флаги).
     */
    enum Modifier
    {
        ABSTRACT = 1,
        FINAL = 1 << 1
    };

    explicit ClassUnit(const std::string& name, Flags flags = 0)
        : m_name(name)
        , m_flags(flags)
    {
    }

    /**
     * @brief Добавляет элемент в нужную секцию доступа.
     * @param unit  вложенный элемент.
     * @param flags модификатор доступа.
     */
    void add(const std::shared_ptr<Unit>& unit, Flags flags) override
    {
        int accessModifier = PRIVATE;
        if (flags <= PRIVATE_PROTECTED) {
            accessModifier = flags;
        }
        unit->setAccessModifier(accessModifier);
        m_fields[accessModifier].push_back(unit);
    }

protected:
    std::string m_name;
    Flags m_flags;
    std::map<int, std::vector<std::shared_ptr<Unit>>> m_fields;
};

#endif
