#include "csharpfactory.h"

#include <vector>

namespace {

const char* const LANGUAGE = "C#";

bool isProtectedAccess(int accessModifier)
{
    return accessModifier == ClassUnit::PROTECTED
        || accessModifier == ClassUnit::PROTECTED_INTERNAL
        || accessModifier == ClassUnit::PRIVATE_PROTECTED;
}

} // namespace

std::string CSharpClassUnit::accessModifierToString(int accessModifier)
{
    switch (accessModifier) {
    case PUBLIC:
        return "public";
    case PROTECTED:
        return "protected";
    case PRIVATE:
        return "private";
    case INTERNAL:
        return "internal";
    case PROTECTED_INTERNAL:
        return "protected internal";
    case PRIVATE_PROTECTED:
        return "private protected";
    case PACKAGE_PRIVATE:
        return "internal";
    default:
        return "";
    }
}

void CSharpClassUnit::check(unsigned int level) const
{
    checkCommonFlags(LANGUAGE);

    // Класс верхнего уровня может быть только public или internal.
    if (level == 0 && m_accessModifier != NO_ACCESS && m_accessModifier != PUBLIC
        && m_accessModifier != INTERNAL && m_accessModifier != PACKAGE_PRIVATE) {
        fail(LANGUAGE, "класс верхнего уровня может быть только public или internal");
    }

    if (m_flags & STATIC) {
        if (m_flags & (ABSTRACT | FINAL)) {
            fail(LANGUAGE, "static-класс не может быть abstract или sealed");
        }
        if (!m_parent.empty()) {
            fail(LANGUAGE, "static-класс не может наследоваться от другого класса");
        }
    }

    for (const auto& method : methods()) {
        const auto flags = method->flags();
        if ((m_flags & STATIC) && !(flags & MethodUnit::STATIC)) {
            fail(LANGUAGE, "в static-классе метод " + method->name() + " тоже должен быть static");
        }
        if ((m_flags & STATIC) && isProtectedAccess(method->accessModifier())) {
            fail(LANGUAGE, "в static-классе метод " + method->name() + " не может быть protected");
        }
        if ((flags & MethodUnit::ABSTRACT) && !(m_flags & ABSTRACT)) {
            fail(LANGUAGE, "абстрактный метод " + method->name() + " допустим только в abstract-классе");
        }
        if ((flags & MethodUnit::VIRTUAL) && (m_flags & FINAL)) {
            fail(LANGUAGE, "в sealed-классе метод " + method->name() + " не может быть virtual");
        }
    }
}

std::string CSharpClassUnit::compile(unsigned int level) const
{
    check(level);

    std::string result = generateShift(level);
    std::string access = accessModifierToString(m_accessModifier);
    if (!access.empty()) {
        result += access + " ";
    }
    if (m_flags & ABSTRACT) {
        result += "abstract ";
    }
    if (m_flags & STATIC) {
        result += "static ";
    }
    if (m_flags & FINAL) {
        result += "sealed ";
    }
    if (m_flags & PARTIAL) {
        result += "partial ";
    }
    result += "class " + m_name;
    if (!m_parent.empty()) {
        result += " : " + m_parent;
    }
    result += " {\n";

    static const std::vector<int> order = {
        PUBLIC, INTERNAL, PROTECTED, PROTECTED_INTERNAL, PRIVATE_PROTECTED, PRIVATE, PACKAGE_PRIVATE
    };
    for (int accessModifier : order) {
        auto it = m_fields.find(accessModifier);
        if (it == m_fields.end()) {
            continue;
        }
        for (const auto& field : it->second) {
            result += field->compile(level + 1);
        }
    }

    result += generateShift(level) + "}\n";
    return result;
}

void CSharpMethodUnit::check() const
{
    checkCommonFlags(LANGUAGE);
    if ((m_flags & VIRTUAL) && (m_flags & (ABSTRACT | OVERRIDE))) {
        fail(LANGUAGE, "virtual нельзя сочетать с abstract или override");
    }
    if ((m_flags & (VIRTUAL | ABSTRACT | OVERRIDE)) && m_accessModifier == ClassUnit::PRIVATE) {
        fail(LANGUAGE, "virtual, abstract и override-методы не могут быть private");
    }
    if (m_flags & ASYNC) {
        if (m_flags & ABSTRACT) {
            fail(LANGUAGE, "abstract-метод не может быть async");
        }
        if (m_returnType != "void" && m_returnType.rfind("Task", 0) != 0) {
            fail(LANGUAGE, "async-метод должен возвращать void, Task или Task<T>");
        }
    }
}

std::string CSharpMethodUnit::compile(unsigned int level) const
{
    check();

    std::string result = generateShift(level);
    std::string access = CSharpClassUnit::accessModifierToString(m_accessModifier);
    if (!access.empty()) {
        result += access + " ";
    }
    if (m_flags & STATIC) {
        result += "static ";
    }
    if (m_flags & ABSTRACT) {
        result += "abstract ";
    }
    if (m_flags & VIRTUAL) {
        result += "virtual ";
    }
    if (m_flags & OVERRIDE) {
        result += "override ";
    }
    if (m_flags & SEALED) {
        result += "sealed ";
    }
    if (m_flags & ASYNC) {
        result += "async ";
    }
    result += m_returnType + " " + m_name + "()";
    if (m_flags & ABSTRACT) {
        return result + ";\n";
    }
    return result + compileBody(level);
}

std::string CSharpPrintOperatorUnit::compile(unsigned int level) const
{
    return generateShift(level) + "Console.Write(\"" + m_text + "\");\n";
}

std::shared_ptr<ClassUnit> CSharpFactory::createClass(const std::string& name, Unit::Flags flags) const
{
    return std::make_shared<CSharpClassUnit>(name, flags);
}

std::shared_ptr<MethodUnit> CSharpFactory::createMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) const
{
    return std::make_shared<CSharpMethodUnit>(name, returnType, flags);
}

std::shared_ptr<PrintOperatorUnit> CSharpFactory::createPrintOperator(const std::string& text) const
{
    return std::make_shared<CSharpPrintOperatorUnit>(text);
}

std::string CSharpFactory::wrapProgram(const std::string& classCode) const
{
    return "using System;\n\n" + classCode + "\nclass Program {\n    static void Main() {\n    }\n}\n";
}

std::string CSharpFactory::fileName() const
{
    return "MyClass.cs";
}
