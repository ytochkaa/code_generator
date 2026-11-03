#include "../cppfactory.h"
#include "../csharpfactory.h"
#include "../javafactory.h"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int failures = 0;

/**
 * @brief Проверяет, что генерация завершается ошибкой std::logic_error.
 */
void expectError(const std::string& title, const std::function<void()>& action)
{
    try {
        action();
        std::cout << "[FAIL] " << title << ": ошибка не обнаружена\n";
        ++failures;
    } catch (const std::logic_error& error) {
        std::cout << "[OK]   " << title << ": " << error.what() << "\n";
    }
}

/**
 * @brief Проверяет, что генерация проходит без ошибок.
 */
void expectSuccess(const std::string& title, const std::function<void()>& action)
{
    try {
        action();
        std::cout << "[OK]   " << title << "\n";
    } catch (const std::logic_error& error) {
        std::cout << "[FAIL] " << title << ": " << error.what() << "\n";
        ++failures;
    }
}

/**
 * @brief Компилирует класс с одним публичным методом.
 */
void compileMethod(const Factory& factory, Unit::Flags methodFlags, Unit::Flags classFlags = 0,
    Unit::Flags access = ClassUnit::PUBLIC, const std::string& returnType = "void")
{
    auto cls = factory.createClass("Test", classFlags);
    cls->add(factory.createMethod("method", returnType, methodFlags), access);
    cls->compile();
}

} // namespace

int main()
{
    const CppFactory cpp;
    const CSharpFactory csharp;
    const JavaFactory java;
    const Factory* all[] = {&cpp, &csharp, &java};

    std::cout << "Общие правила (все языки)\n";
    for (const Factory* factory : all) {
        const std::string lang = factory->fileName();
        expectError(lang + ": static + virtual", [&] { compileMethod(*factory, MethodUnit::STATIC | MethodUnit::VIRTUAL); });
        expectError(lang + ": static + abstract", [&] { compileMethod(*factory, MethodUnit::STATIC | MethodUnit::ABSTRACT, ClassUnit::ABSTRACT); });
        expectError(lang + ": abstract + final", [&] { compileMethod(*factory, MethodUnit::ABSTRACT | MethodUnit::FINAL, ClassUnit::ABSTRACT); });
        expectError(lang + ": sealed без override", [&] { compileMethod(*factory, MethodUnit::SEALED); });
        expectError(lang + ": класс abstract + final", [&] { compileMethod(*factory, 0, ClassUnit::ABSTRACT | ClassUnit::FINAL); });
        expectError(lang + ": override без базового класса", [&] { compileMethod(*factory, MethodUnit::OVERRIDE); });
        expectSuccess(lang + ": override + sealed в наследнике", [&] {
            auto cls = factory->createClass("Test");
            cls->setParent("Base");
            cls->add(factory->createMethod("method", "void", MethodUnit::OVERRIDE | MethodUnit::SEALED), ClassUnit::PUBLIC);
            cls->compile();
        });
    }

    std::cout << "\nC++\n";
    expectError("static + const", [&] { compileMethod(cpp, MethodUnit::STATIC | MethodUnit::CONST); });
    expectSuccess("abstract-метод в классе без ABSTRACT (чисто виртуальный)", [&] { compileMethod(cpp, MethodUnit::ABSTRACT); });

    std::cout << "\nC#\n";
    expectError("abstract-метод в неабстрактном классе", [&] { compileMethod(csharp, MethodUnit::ABSTRACT); });
    expectError("virtual + override", [&] {
        auto cls = csharp.createClass("Test");
        cls->setParent("Base");
        cls->add(csharp.createMethod("method", "void", MethodUnit::VIRTUAL | MethodUnit::OVERRIDE), ClassUnit::PUBLIC);
        cls->compile();
    });
    expectError("virtual + abstract", [&] { compileMethod(csharp, MethodUnit::VIRTUAL | MethodUnit::ABSTRACT, ClassUnit::ABSTRACT); });
    expectError("private virtual", [&] { compileMethod(csharp, MethodUnit::VIRTUAL, 0, ClassUnit::PRIVATE); });
    expectError("virtual в sealed-классе", [&] { compileMethod(csharp, MethodUnit::VIRTUAL, ClassUnit::FINAL); });
    expectError("нестатический метод в static-классе", [&] { compileMethod(csharp, 0, ClassUnit::STATIC); });
    expectError("protected-метод в static-классе", [&] { compileMethod(csharp, MethodUnit::STATIC, ClassUnit::STATIC, ClassUnit::PROTECTED); });
    expectError("static + abstract класс", [&] { compileMethod(csharp, MethodUnit::STATIC, ClassUnit::STATIC | ClassUnit::ABSTRACT); });
    expectError("async + abstract", [&] { compileMethod(csharp, MethodUnit::ASYNC | MethodUnit::ABSTRACT, ClassUnit::ABSTRACT); });
    expectError("async с типом int", [&] { compileMethod(csharp, MethodUnit::ASYNC, 0, ClassUnit::PUBLIC, "int"); });
    expectError("private класс верхнего уровня", [&] {
        auto cls = csharp.createClass("Test");
        cls->setAccessModifier(ClassUnit::PRIVATE);
        cls->compile();
    });
    expectError("static-класс с базовым классом", [&] {
        auto cls = csharp.createClass("Test", ClassUnit::STATIC);
        cls->setParent("Base");
        cls->compile();
    });
    expectSuccess("static-класс со static-методом", [&] { compileMethod(csharp, MethodUnit::STATIC, ClassUnit::STATIC); });
    expectSuccess("async void", [&] { compileMethod(csharp, MethodUnit::ASYNC); });
    expectSuccess("private protected вложенный класс", [&] {
        auto outer = csharp.createClass("Outer");
        outer->add(csharp.createClass("Inner"), ClassUnit::PRIVATE_PROTECTED);
        outer->compile();
    });

    std::cout << "\nJava\n";
    expectError("abstract-метод в неабстрактном классе", [&] { compileMethod(java, MethodUnit::ABSTRACT); });
    expectError("private abstract", [&] { compileMethod(java, MethodUnit::ABSTRACT, ClassUnit::ABSTRACT, ClassUnit::PRIVATE); });
    expectError("static класс верхнего уровня", [&] { java.createClass("Test", ClassUnit::STATIC)->compile(); });
    expectError("protected класс верхнего уровня", [&] {
        auto cls = java.createClass("Test");
        cls->setAccessModifier(ClassUnit::PROTECTED);
        cls->compile();
    });
    expectSuccess("static вложенный класс", [&] {
        auto outer = java.createClass("Outer");
        outer->add(java.createClass("Inner", ClassUnit::STATIC), ClassUnit::PRIVATE);
        outer->compile();
    });

    std::cout << "\n" << (failures == 0 ? "Все проверки пройдены" : "Есть ошибки: " + std::to_string(failures)) << "\n";
    return failures == 0 ? 0 : 1;
}
