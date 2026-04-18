#include "cppfactory.h"

#include <iostream>
#include <memory>

/**
 * @brief Строит дерево элементов класса и генерирует программу.
 *
 * Функция не знает целевого языка — он определяется переданной фабрикой.
 * @param factory фабрика элементов нужного языка.
 */
std::string generateProgram(const Factory& factory)
{
    auto myClass = factory.createClass("MyClass");

    myClass->add(factory.createMethod("testFunc1", "void", 0), ClassUnit::PUBLIC);
    myClass->add(factory.createMethod("testFunc2", "void", MethodUnit::STATIC), ClassUnit::PRIVATE);
    myClass->add(factory.createMethod("testFunc3", "void", MethodUnit::VIRTUAL | MethodUnit::CONST), ClassUnit::PUBLIC);

    auto testFunc4 = factory.createMethod("testFunc4", "void", MethodUnit::STATIC);
    testFunc4->add(factory.createPrintOperator(R"(Hello, world!\n)"));
    myClass->add(testFunc4, ClassUnit::PROTECTED);

    return factory.wrapProgram(myClass->compile());
}

int main()
{
    CppFactory factory;
    std::cout << generateProgram(factory) << std::endl;
    return 0;
}
