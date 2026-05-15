#ifndef PRINTOPERATORUNIT_H
#define PRINTOPERATORUNIT_H

#include "unit.h"

/**
 * @brief Абстрактный элемент "оператор печати".
 *
 * Хранит выводимый текст; конкретный синтаксис вывода задаётся наследником
 * для каждого языка.
 */
class PrintOperatorUnit : public Unit
{
public:
    explicit PrintOperatorUnit(const std::string& text)
        : m_text(text)
    {
    }

protected:
    std::string m_text;
};

#endif
