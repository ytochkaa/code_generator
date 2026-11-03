#include "cppfactory.h"
#include "csharpfactory.h"
#include "javafactory.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

/**
 * @brief Строит дерево элементов классов и генерирует программу.
 *
 * Функция не знает целевого языка — он определяется переданной фабрикой.
 * @param factory фабрика элементов нужного языка.
 * @throw std::logic_error если модификаторы недопустимы для целевого языка.
 */
std::string generateProgram(const Factory& factory)
{
    auto myClass = factory.createClass("MyClass", ClassUnit::ABSTRACT);
    myClass->setAccessModifier(ClassUnit::PUBLIC);

    myClass->add(factory.createMethod("draw", "void", MethodUnit::VIRTUAL), ClassUnit::PUBLIC);
    myClass->add(factory.createMethod("testFunc1", "void", 0), ClassUnit::PUBLIC);
    myClass->add(factory.createMethod("testFunc2", "void", MethodUnit::STATIC | MethodUnit::FINAL), ClassUnit::PRIVATE);
    myClass->add(factory.createMethod("testFunc3", "void", MethodUnit::VIRTUAL | MethodUnit::CONST), ClassUnit::PUBLIC);

    auto testFunc4 = factory.createMethod("testFunc4", "void", MethodUnit::STATIC);
    testFunc4->add(factory.createPrintOperator(R"(Hello, world!\n)"));
    myClass->add(testFunc4, ClassUnit::PROTECTED);

    myClass->add(factory.createMethod("testFunc5", "void", MethodUnit::ABSTRACT), ClassUnit::PUBLIC);
    myClass->add(factory.createMethod("testFunc6", "void", 0), ClassUnit::INTERNAL);

    auto childClass = factory.createClass("ChildClass", ClassUnit::FINAL);
    childClass->setAccessModifier(ClassUnit::INTERNAL);
    childClass->setParent("MyClass");

    childClass->add(factory.createMethod("draw", "void", MethodUnit::OVERRIDE | MethodUnit::SEALED), ClassUnit::PUBLIC);
    childClass->add(factory.createMethod("testFunc5", "void", MethodUnit::OVERRIDE), ClassUnit::PUBLIC);

    return factory.wrapProgram(myClass->compile() + "\n" + childClass->compile());
}

int main()
{
    std::vector<std::shared_ptr<Factory>> factories = {
        std::make_shared<CppFactory>(),
        std::make_shared<CSharpFactory>(),
        std::make_shared<JavaFactory>()
    };

    std::filesystem::create_directory("generated");

    int result = 0;
    for (const auto& factory : factories) {
        try {
            std::string program = generateProgram(*factory);
            std::cout << program << "\n";

            std::ofstream out("generated/" + factory->fileName());
            out << program;
        } catch (const std::logic_error& error) {
            std::cerr << "Ошибка генерации " << factory->fileName() << ": " << error.what() << "\n";
            result = 1;
        }
    }

    return result;
}
