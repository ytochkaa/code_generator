#ifndef UNIT_H
#define UNIT_H

#include <memory>
#include <stdexcept>
#include <string>

/**
 * @brief Базовый абстрактный элемент синтаксиса генерируемого кода.
 *
 * От него наследуются все конкретные элементы (класс, метод, оператор
 * печати) во всех поддерживаемых языках.
 */
class Unit
{
public:
    using Flags = unsigned int;

    virtual ~Unit() = default;

    /**
     * @brief Добавляет вложенный элемент.
     * @param unit  вложенный элемент.
     * @param flags модификатор доступа (для класса) либо игнорируется.
     */
    virtual void add(const std::shared_ptr<Unit>&, Flags)
    {
        throw std::runtime_error("Not supported");
    }

    /**
     * @brief Генерирует исходный код элемента.
     * @param level уровень вложенности для расстановки отступов.
     */
    virtual std::string compile(unsigned int level = 0) const = 0;

    void setAccessModifier(int accessModifier) { m_accessModifier = accessModifier; }
    int accessModifier() const { return m_accessModifier; }

protected:
    /**
     * @brief Возвращает отступ, соответствующий уровню вложенности.
     */
    std::string generateShift(unsigned int level) const
    {
        std::string result;
        for (unsigned int i = 0; i < level; ++i) {
            result += "    ";
        }
        return result;
    }

    int m_accessModifier = 0;
};

#endif
