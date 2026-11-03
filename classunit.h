#ifndef CLASSUNIT_H
#define CLASSUNIT_H

#include "methodunit.h"
#include "unit.h"

#include <map>
#include <stdexcept>
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
    /**
     * @brief Проверяет сочетания модификаторов класса, недопустимые во всех языках.
     * @param language название языка для текста ошибки.
     * @throw std::logic_error при недопустимом сочетании.
     */
    void checkCommonFlags(const std::string& language) const
    {
        if ((m_flags & ABSTRACT) && (m_flags & FINAL)) {
            fail(language, "abstract нельзя сочетать с final (sealed)");
        }
        for (const auto& method : methods()) {
            if ((method->flags() & MethodUnit::OVERRIDE) && m_parent.empty()) {
                fail(language, "метод " + method->name() + " помечен override, но у класса нет базового класса");
            }
        }
    }

    /**
     * @brief Возвращает все методы класса (без вложенных классов и прочих элементов).
     */
    std::vector<std::shared_ptr<MethodUnit>> methods() const
    {
        std::vector<std::shared_ptr<MethodUnit>> result;
        for (const auto& section : m_fields) {
            for (const auto& unit : section.second) {
                if (auto method = std::dynamic_pointer_cast<MethodUnit>(unit)) {
                    result.push_back(method);
                }
            }
        }
        return result;
    }

    /**
     * @brief Выбрасывает исключение с описанием ошибки в классе.
     */
    [[noreturn]] void fail(const std::string& language, const std::string& message) const
    {
        throw std::logic_error(language + ": класс " + m_name + ": " + message);
    }

    std::string m_name;
    Flags m_flags;
    std::string m_parent;
    std::map<int, std::vector<std::shared_ptr<Unit>>> m_fields;
};

#endif
