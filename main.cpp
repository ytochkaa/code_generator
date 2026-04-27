#include "cppfactory.h"
#include "csharpfactory.h"
#include "javafactory.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

/**
 * @brief Строит дерево элементов класса и генерирует программу.
 *
 * Функция не знает целевого языка — он определяется переданной фабрикой.
 * @param factory фабрика элементов нужного языка.
 */
std::string generateProgram(const Factory& factory)
{
    auto baseClass = factory.createClass("BaseClass");
    baseClass->add(factory.createMethod("draw", "void", MethodUnit::VIRTUAL), ClassUnit::PUBLIC);

    auto myClass = factory.createClass("MyClass", ClassUnit::ABSTRACT);
    myClass->setParent("BaseClass");

    myClass->add(factory.createMethod("draw", "void", MethodUnit::SEALED), ClassUnit::PUBLIC);
    myClass->add(factory.createMethod("testFunc1", "void", 0), ClassUnit::PUBLIC);
    myClass->add(factory.createMethod("testFunc2", "void", MethodUnit::STATIC | MethodUnit::FINAL), ClassUnit::PRIVATE);
    myClass->add(factory.createMethod("testFunc3", "void", MethodUnit::VIRTUAL | MethodUnit::CONST), ClassUnit::PUBLIC);

    auto testFunc4 = factory.createMethod("testFunc4", "void", MethodUnit::STATIC);
    testFunc4->add(factory.createPrintOperator(R"(Hello, world!\n)"));
    myClass->add(testFunc4, ClassUnit::PROTECTED);

    myClass->add(factory.createMethod("testFunc5", "void", MethodUnit::ABSTRACT), ClassUnit::PUBLIC);
    myClass->add(factory.createMethod("testFunc6", "void", 0), ClassUnit::INTERNAL);

    return factory.wrapProgram(baseClass->compile() + "\n" + myClass->compile());
}

int main()
{
    std::vector<std::shared_ptr<Factory>> factories = {
        std::make_shared<CppFactory>(),
        std::make_shared<CSharpFactory>(),
        std::make_shared<JavaFactory>()
    };

    std::filesystem::create_directory("generated");

    for (const auto& factory : factories) {
        std::string program = generateProgram(*factory);
        std::cout << program << "\n";

        std::ofstream out("generated/" + factory->fileName());
        out << program;
    }

    return 0;
}
