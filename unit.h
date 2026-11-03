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

    /**
     * @brief Значение модификатора доступа "не задан".
     */
    static constexpr int NO_ACCESS = -1;

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
     * @throw std::logic_error если сочетание модификаторов недопустимо
     *        для целевого языка.
     */
    virtual std::string compile(unsigned int level = 0) const = 0;

    /**
     * @brief Задаёт модификатор доступа элемента.
     *
     * Для членов класса вызывается из ClassUnit::add, для класса верхнего
     * уровня — вручную.
     */
    void setAccessModifier(int accessModifier) { m_accessModifier = accessModifier; }
    int accessModifier() const { return m_accessModifier; }

protected:
    /**
     * @brief Возвращает отступ, соответствующий уровню вложенности.
     */
    virtual std::string generateShift(unsigned int level) const
    {
        static const auto DEFAULT_SHIFT = "    ";
        std::string result;
        for (unsigned int i = 0; i < level; ++i) {
            result += DEFAULT_SHIFT;
        }
        return result;
    }

    int m_accessModifier = NO_ACCESS;
};

#endif
