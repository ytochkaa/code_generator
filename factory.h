#ifndef FACTORY_H
#define FACTORY_H

#include "classunit.h"
#include "methodunit.h"
#include "printoperatorunit.h"

#include <memory>
#include <string>

/**
 * @brief Абстрактная фабрика элементов кода.
 *
 * Каждый конкретный наследник создаёт элементы для своего языка и
 * оборачивает сгенерированный класс в законченную программу.
 */
class Factory
{
public:
    virtual ~Factory() = default;

    /**
     * @brief Создаёт элемент "класс".
     */
    virtual std::shared_ptr<ClassUnit> createClass(const std::string& name, Unit::Flags flags = 0) const = 0;

    /**
     * @brief Создаёт элемент "функция-член".
     */
    virtual std::shared_ptr<MethodUnit> createMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) const = 0;

    /**
     * @brief Создаёт элемент "оператор печати".
     */
    virtual std::shared_ptr<PrintOperatorUnit> createPrintOperator(const std::string& text) const = 0;

    /**
     * @brief Оборачивает сгенерированный класс в законченную программу.
     */
    virtual std::string wrapProgram(const std::string& classCode) const = 0;

    /**
     * @brief Имя файла для сохранения сгенерированной программы.
     */
    virtual std::string fileName() const = 0;
};

#endif
