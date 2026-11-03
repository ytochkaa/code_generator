#include "javafactory.h"

#include <vector>

namespace {

const char* const LANGUAGE = "Java";

} // namespace

std::string JavaClassUnit::accessModifierToString(int accessModifier)
{
    switch (accessModifier) {
    case PUBLIC:
        return "public";
    case PROTECTED:
        return "protected";
    case PROTECTED_INTERNAL:
        return "protected";
    case PRIVATE:
        return "private";
    case PRIVATE_PROTECTED:
        return "private";
    case INTERNAL:
        return "";
    case PACKAGE_PRIVATE:
        return "";
    default:
        return "";
    }
}

void JavaClassUnit::check(unsigned int level) const
{
    checkCommonFlags(LANGUAGE);

    if (level == 0) {
        // Класс верхнего уровня может быть только public или package-private.
        if (m_accessModifier == PROTECTED || m_accessModifier == PROTECTED_INTERNAL
            || m_accessModifier == PRIVATE || m_accessModifier == PRIVATE_PROTECTED) {
            fail(LANGUAGE, "класс верхнего уровня может быть только public или package-private");
        }
        if (m_flags & STATIC) {
            fail(LANGUAGE, "static допустим только у вложенного класса");
        }
    }

    for (const auto& method : methods()) {
        if ((method->flags() & MethodUnit::ABSTRACT) && !(m_flags & ABSTRACT)) {
            fail(LANGUAGE, "абстрактный метод " + method->name() + " допустим только в abstract-классе");
        }
    }
}

std::string JavaClassUnit::compile(unsigned int level) const
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
        result += "final ";
    }
    result += "class " + m_name;
    if (!m_parent.empty()) {
        result += " extends " + m_parent;
    }
    result += " {\n";

    static const std::vector<int> order = {
        PUBLIC, PROTECTED, PROTECTED_INTERNAL, PRIVATE, PRIVATE_PROTECTED, INTERNAL, PACKAGE_PRIVATE
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

void JavaMethodUnit::check() const
{
    checkCommonFlags(LANGUAGE);
    if ((m_flags & (ABSTRACT | OVERRIDE)) && m_accessModifier == ClassUnit::PRIVATE) {
        fail(LANGUAGE, "abstract и override-методы не могут быть private");
    }
}

std::string JavaMethodUnit::compile(unsigned int level) const
{
    check();

    std::string result;
    if (m_flags & OVERRIDE) {
        result += generateShift(level) + "@Override\n";
    }
    result += generateShift(level);
    std::string access = JavaClassUnit::accessModifierToString(m_accessModifier);
    if (!access.empty()) {
        result += access + " ";
    }
    if (m_flags & STATIC) {
        result += "static ";
    }
    if (m_flags & ABSTRACT) {
        result += "abstract ";
    }
    // Аналог sealed-метода C# в Java — final-метод.
    if (m_flags & (FINAL | SEALED)) {
        result += "final ";
    }
    result += m_returnType + " " + m_name + "()";
    if (m_flags & ABSTRACT) {
        return result + ";\n";
    }
    return result + compileBody(level);
}

std::string JavaPrintOperatorUnit::compile(unsigned int level) const
{
    return generateShift(level) + "System.out.print(\"" + m_text + "\");\n";
}

std::shared_ptr<ClassUnit> JavaFactory::createClass(const std::string& name, Unit::Flags flags) const
{
    return std::make_shared<JavaClassUnit>(name, flags);
}

std::shared_ptr<MethodUnit> JavaFactory::createMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) const
{
    return std::make_shared<JavaMethodUnit>(name, returnType, flags);
}

std::shared_ptr<PrintOperatorUnit> JavaFactory::createPrintOperator(const std::string& text) const
{
    return std::make_shared<JavaPrintOperatorUnit>(text);
}

std::string JavaFactory::wrapProgram(const std::string& classCode) const
{
    return classCode + "\nclass Program {\n    public static void main(String[] args) {\n    }\n}\n";
}

std::string JavaFactory::fileName() const
{
    return "MyClass.java";
}
